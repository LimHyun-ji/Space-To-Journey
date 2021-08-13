// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_FreeFly.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "PlayerCPP.h"
#include "PlayerBullet.h"
#include "Enemy_BulletBall.h"
#include <Components/WidgetComponent.h>
#include "EnemyHpBar_Widget.h"
#include "TargetDisplayWidget.h"
#include "Item.h"
#include "Explosion.h"
#include "Bullet_Missile.h"
#include "AllBulletParent.h"

// Sets default values
AEnemy_FreeFly::AEnemy_FreeFly(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	firePosition1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position1"));
	firePosition1->SetupAttachment(collComp);

	firePosition2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position2"));
	firePosition2->SetupAttachment(collComp);

}

// Called when the game starts or when spawned
void AEnemy_FreeFly::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AEnemy_FreeFly::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(target))
	{
		targetDistance = GetDistanceTo(target);

		if (isFireActivation)
		{
			FireWork();
		}

		if (targetDistance < fireActivateDistance)
		{
			isFireActivation = true;
		}
		else
		{
			isFireActivation = false;
		}


		if (isTrackActivation)
		{
			Turn();

			if (targetDistance > stopDistance)
			{
				Move();
			}
		}

		if (targetDistance < trackActivateDistance && !isTrackActivation)
		{
			isTrackActivation = true;
		}
	}

	if (currentHp <= 0)
	{
		Dead();
	}
	//removeByThisPlayer = NULL;

	if (isHpBarOpacityWork)
	{
		HpBarOpacityWork();
	}

	if (isReloading)
	{
		Reloading();
	}

	ActivateTargetDisplay();

	MoveTargetDisplay();
}

void AEnemy_FreeFly::Move()
{
	dir = GetActorForwardVector();
	FVector P0 = GetActorLocation();
	FVector vel = speed * GetWorld()->DeltaTimeSeconds * dir;
	FVector P = vel + P0;

	SetActorLocation(P, true);
}

void AEnemy_FreeFly::Turn()
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

}

void AEnemy_FreeFly::Fire()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, GetActorLocation());

	float spread_Pitch1 = FMath::RandRange(-spread, spread);
	float spread_Yaw1 = FMath::RandRange(-spread, spread);
	float spread_Pitch2 = FMath::RandRange(-spread, spread);
	float spread_Yaw2 = FMath::RandRange(-spread, spread);

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AEnemy_BulletBall>(bulletFactory, firePosition1->GetComponentLocation(), firePosition1->GetComponentRotation() + FRotator(spread_Pitch1, spread_Yaw1, 0), param);
	GetWorld()->SpawnActor<AEnemy_BulletBall>(bulletFactory, firePosition2->GetComponentLocation(), firePosition2->GetComponentRotation() + FRotator(spread_Pitch2, spread_Yaw2, 0), param);
}

void AEnemy_FreeFly::Dead()
{

	DropItem();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploreFactory, GetActorLocation(), GetActorRotation(), GetActorScale3D() + FVector(2, 2, 2));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), deadSound, GetActorLocation());
	
	if (removeByThisPlayer != NULL)
	{		
		UE_LOG(LogTemp, Warning, TEXT("name : %s"), *removeByThisPlayer->GetName());
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
	

	Destroy();
}


void AEnemy_FreeFly::OnCollisionEnter_Overlab(AActor* OtherActor)
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
		SetRemoveByThisPlayer(playerBullet->GetBulletOwner());
		//if(IsValid(Cast<APlayerCPP>(playerBullet->GetBulletOwner())))	removeByThisPlayer = playerBullet->GetBulletOwner();

	}
	else if (IsValid(player))
	{		
		player->TakeDamage(collisionDamage);
		Dead();
	}


}



