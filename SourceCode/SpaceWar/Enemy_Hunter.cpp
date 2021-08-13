// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Hunter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "PlayerCPP.h"
#include "Enemy_BulletBasic.h"
#include "PlayerBullet.h"
#include "Item.h"
#include <Components/WidgetComponent.h>
#include "EnemyHpBar_Widget.h"
#include "TargetDisplayWidget.h"
#include "Explosion.h"
#include "Bullet_Missile.h"
#include "AllBulletParent.h"

// Sets default values
AEnemy_Hunter::AEnemy_Hunter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	firePosition1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position1"));
	firePosition1->SetupAttachment(collComp);

}

// Called when the game starts or when spawned
void AEnemy_Hunter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy_Hunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(target))
	{
		if (isFireActivation)
		{
			FireWork();
		}


		float distance = GetDistanceTo(target);
		if (distance < 3000 && !isAvoid)
		{
			isAvoid = true;
			isFireActivation = false;
			avoidTurn_Pitch = FMath::RandRange(-40, 40);
			avoidTurn_Pitch += avoidTurn_Pitch / abs(avoidTurn_Pitch) * 10;
			avoidTurn_Yaw = FMath::RandRange(-40, 40);
			avoidTurn_Yaw += avoidTurn_Yaw / abs(avoidTurn_Yaw) * 10;
		}

		if (isTrackPlayer)
		{
			if (isAvoid)
			{
				Avoid();
			}
			else
			{
				Turn();
			}

			Move();
		}
		else if (distance < trackPlayerDistance)
		{
			isTrackPlayer = true;
		}

	}

	if (currentHp <= 0)
	{
		Dead();
	}
	removeByThisPlayer = NULL;

	if (isHpBarOpacityWork)
	{
		HpBarOpacityWork();
	}

	ActivateTargetDisplay();

	MoveTargetDisplay();
}

void AEnemy_Hunter::Destroyed()
{
	Super::Destroyed();
	/*


	*/

}

void AEnemy_Hunter::Turn()
{
	FVector currentLocation = GetActorLocation();
	FVector targetLocation = target->GetActorLocation();

	FRotator goalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);
	FRotator newRotation;

	if (goalRotation.Yaw < GetActorRotation().Yaw)
	{
		if (abs(GetActorRotation().Yaw - goalRotation.Yaw) < 0.5)
		{
			newRotation.Yaw = GetActorRotation().Yaw;
		}
		else
		{
			newRotation.Yaw = GetActorRotation().Yaw - turnSpeed;
		}

	}
	else if (goalRotation.Yaw > GetActorRotation().Yaw)
	{

		if (abs(GetActorRotation().Yaw - goalRotation.Yaw) < 0.5)
		{
			newRotation.Yaw = GetActorRotation().Yaw;
		}
		else
		{
			newRotation.Yaw = GetActorRotation().Yaw + turnSpeed;
		}

	}

	if (GetActorRotation().Yaw - goalRotation.Yaw > 180)
	{
		newRotation.Yaw = GetActorRotation().Yaw + turnSpeed;
	}
	else if (GetActorRotation().Yaw - goalRotation.Yaw < -180)
	{
		newRotation.Yaw = GetActorRotation().Yaw - turnSpeed;
	}


	////////////////

	if (goalRotation.Pitch < GetActorRotation().Pitch)
	{
		if (abs(GetActorRotation().Pitch - goalRotation.Pitch) < 0.5)
		{
			newRotation.Pitch = GetActorRotation().Pitch;
		}
		else
		{
			newRotation.Pitch = GetActorRotation().Pitch - turnSpeed;
		}

	}
	else if (goalRotation.Pitch > GetActorRotation().Pitch)
	{

		if (abs(GetActorRotation().Pitch - goalRotation.Pitch) < 0.5)
		{
			newRotation.Pitch = GetActorRotation().Pitch;
		}
		else
		{
			newRotation.Pitch = GetActorRotation().Pitch + turnSpeed;
		}


	}

	SetActorRotation(FRotator(newRotation.Pitch, newRotation.Yaw, 0));



	if ((abs(GetActorRotation().Pitch - goalRotation.Pitch) < 2) && (abs(GetActorRotation().Yaw - goalRotation.Yaw) < 2) && GetDistanceTo(target) < fireActivationDistance)
	{
		isFireActivation = true;
	}
	else
	{
		isFireActivation = false;
	}

}

void AEnemy_Hunter::Avoid()
{
	currentAvoidTime += GetWorld()->DeltaTimeSeconds;

	if (currentAvoidTime > avoidTime)
	{
		currentAvoidTime = 0;
		currentAvoidTurn_Pitch = 0;
		currentAvoidTurn_Yaw = 0;
		isAvoid = false;
	}

	FRotator newRotation;

	if (abs(currentAvoidTurn_Pitch) < abs(avoidTurn_Pitch))
	{
		currentAvoidTurn_Pitch += avoidTurn_Pitch / abs(avoidTurn_Pitch);
		newRotation.Pitch = avoidTurn_Pitch / abs(avoidTurn_Pitch);
	}
	else
	{
		newRotation.Pitch = 0;
	}

	if (abs(currentAvoidTurn_Yaw) < abs(avoidTurn_Yaw))
	{
		currentAvoidTurn_Yaw += avoidTurn_Yaw / abs(avoidTurn_Yaw);
		newRotation.Yaw = avoidTurn_Yaw / abs(avoidTurn_Yaw);
	}
	else
	{
		newRotation.Yaw = 0;
	}

	SetActorRotation(GetActorRotation() + FRotator(newRotation.Pitch, newRotation.Yaw, 0));

}

void AEnemy_Hunter::Move()
{
	dir = GetActorForwardVector();
	FVector P0 = GetActorLocation();
	FVector vel = speed * GetWorld()->DeltaTimeSeconds * dir;
	FVector P = vel + P0;

	SetActorLocation(P, true);
}


void AEnemy_Hunter::Fire()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, GetActorLocation());

	float spread_Pitch = FMath::RandRange(-spread, spread);
	float spread_Yaw = FMath::RandRange(-spread, spread);

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AEnemy_BulletBasic>(bulletFactory, firePosition1->GetComponentLocation(), firePosition1->GetComponentRotation() + FRotator(spread_Pitch, spread_Yaw, 0), param);
}


void AEnemy_Hunter::Dead()
{
	
	DropItem();

	if (removeByThisPlayer != NULL)
	{
		target->RandomVictorySound();

		uint8 count = 0;
		for (FQuestStruct temp : removeByThisPlayer->questData)
		{
			if (removeByThisPlayer->questData[count].targetEnemyNum1 == EnemyNumber && removeByThisPlayer->questData[count].RemoveGoal1 > 0)
			{
				removeByThisPlayer->questData[count].RemoveGoal1--;
			}
			count++;
		}
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploreFactory, GetActorLocation(), GetActorRotation(), GetActorScale3D() + FVector(2, 2, 2));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), deadSound, GetActorLocation());
	Destroy();
}

void AEnemy_Hunter::OnCollisionEnter_Overlab(AActor* OtherActor)
{
	APlayerBullet* playerBullet = Cast<APlayerBullet>(OtherActor);
	APlayerCPP* player = Cast<APlayerCPP>(OtherActor);
	AExplosion* explosion = Cast<AExplosion>(OtherActor);
	ABullet_Missile* missile = Cast<ABullet_Missile>(OtherActor);

	if (IsValid(playerBullet))
	{
		SetHpBarOpacityWork();
		currentHp -= playerBullet->GetBulletDamage();
		playerBullet->ExploreEffect();
		playerBullet->Destroy();
	}
	else if (IsValid(player))
	{		
		player->TakeDamage(collisionDamage);
		Dead();
	}


}



