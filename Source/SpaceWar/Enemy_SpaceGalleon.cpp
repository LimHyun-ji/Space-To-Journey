// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_SpaceGalleon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystem.h"
#include "PlayerCPP.h"
#include "PlayerBullet.h"
#include "Enemy_BulletBasic.h"
#include <Components/WidgetComponent.h>
#include "EnemyHpBar_Widget.h"
#include "Item.h"
#include "TargetDisplayWidget.h"
#include "Explosion.h"
#include "Bullet_Missile.h"
#include "AllBulletParent.h"

// Sets default values
AEnemy_SpaceGalleon::AEnemy_SpaceGalleon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
void AEnemy_SpaceGalleon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy_SpaceGalleon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	Move();

	if (IsValid(target))
	{
		FirePositionLookTarget();
		FireActivate();

		if (!isFireActivation && currentMagazine != magazine)
		{
			isReloading = true;
		}

		if (isReloading)
		{
			Reloading();
		}
		else if (isFireActivation)
		{
			FireWork();
		}
	}


	if (currentHp <= 0)
	{
		Dead();
	}
//	removeByThisPlayer = NULL;

	if (isHpBarOpacityWork)
	{
		HpBarOpacityWork();
	}

	ActivateTargetDisplay();

	MoveTargetDisplay();
}

void AEnemy_SpaceGalleon::Move()
{
	dir = GetActorForwardVector();
	dir.Normalize();

	FVector P0 = GetActorLocation();
	FVector vel = speed * GetWorld()->DeltaTimeSeconds * dir;
	FVector P = vel + P0;

	SetActorLocation(P);
}

void AEnemy_SpaceGalleon::Dead()
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

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), deadSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploreFactory, GetActorLocation(), GetActorRotation(), GetActorScale3D() + FVector(10, 10, 10));
	Destroy();
}



void AEnemy_SpaceGalleon::Fire()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, GetActorLocation());

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
 
	if (bulletFactory->GetName().Contains("Missile"))
	{
		ABullet_Missile* bullet = GetWorld()->SpawnActor<ABullet_Missile>(bulletFactory, firePosition1->GetComponentLocation(), firePosition1->GetComponentRotation() + FRotator(FMath::RandRange(-10,10), FMath::RandRange(-50, 50),0), param);
		bullet->SetBulletSpeed(bulletSpeed);

		if (!isRealTimeHoming)
		{
			bullet->SetLifeTime((GetDistanceTo(target) / bulletSpeed) + FMath::RandRange(-0.2f, 0.2f));
			bullet->SetHomingMissile(target->GetActorLocation() + FVector(FMath::RandRange(-100, 100), FMath::RandRange(-100, 100), 0), 1);
		}
		else
		{
			bullet->SetLifeTime(5 + FMath::RandRange(-0.2f, 0.2f), false);
			bullet->SetRealTimeHomingMissile(target, 1);
			bullet->SetIsIncreaseHomingAbility(false);
		}
		
		return;
	}


	GetWorld()->SpawnActor<AAllBulletParent>(bulletFactory, firePosition1->GetComponentLocation() + FVector(0, 20, 0), firePosition1->GetComponentRotation(), param);
	GetWorld()->SpawnActor<AAllBulletParent>(bulletFactory, firePosition1->GetComponentLocation() + FVector(0, -20, 0), firePosition1->GetComponentRotation(), param);
	GetWorld()->SpawnActor<AAllBulletParent>(bulletFactory, firePosition2->GetComponentTransform(), param);
}

void AEnemy_SpaceGalleon::FirePositionLookTarget()
{
	FVector firePosition1Location = firePosition1->GetComponentLocation();
	FVector firePosition2Location = firePosition2->GetComponentLocation();
	FVector targetLocation = target->GetActorLocation();

	FRotator FirePosition1newRotation = UKismetMathLibrary::FindLookAtRotation(firePosition1Location, targetLocation);
	FRotator FirePosition2newRotation = UKismetMathLibrary::FindLookAtRotation(firePosition2Location, targetLocation);
	firePosition1->SetWorldRotation(FirePosition1newRotation);
	firePosition2->SetWorldRotation(FirePosition2newRotation);
}


void AEnemy_SpaceGalleon::FireActivate()
{
	float distance = GetDistanceTo(target);

	if (distance < fireActivationDistance)
	{
		isFireActivation = true;
	}
	else
	{
		isFireActivation = false;
	}
}

void AEnemy_SpaceGalleon::OnCollisionEnter_Overlab(AActor* OtherActor)
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
		player->ExploreEffect();
		player->Destroy();
	}


}

