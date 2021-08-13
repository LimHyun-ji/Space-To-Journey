// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss_Demon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "EnemyBullet_Parent.h"
#include "AllBulletParent.h"
#include "Bullet_Missile.h"
#include "Player_Human.h"

AEnemyBoss_Demon::AEnemyBoss_Demon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = collComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	firePosition1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position1"));
	firePosition1->SetupAttachment(collComp);
}

// Called when the game starts or when spawned
void AEnemyBoss_Demon::BeginPlay()
{
	Super::BeginPlay();


	TArray <AActor*> obj;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Human::StaticClass(), obj);

	for (AActor* actor : obj)
	{
		APlayer_Human* currentActor = Cast<APlayer_Human>(actor);
		if (IsValid(currentActor))
		{
			target = currentActor;
			break;
		}
	}
}

// Called every frame
void AEnemyBoss_Demon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	targetDistance = GetDistanceTo(target);

	FIreWorkHomingMissile();
}

void AEnemyBoss_Demon::FIreWorkHomingMissile()
{
	currentFireHomingMissileRate += GetWorld()->DeltaTimeSeconds;

	if (currentFireHomingMissileRate > fireHomingMissileRate)
	{
		currentFireHomingMissileRate = 0;
		FireHomingMissile();
	}
}

void AEnemyBoss_Demon::FireHomingMissile()
{
	float spread_Pitch = FMath::RandRange(-spread, spread);
	float spread_Yaw = FMath::RandRange(-spread, spread);

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABullet_Missile* bullet = GetWorld()->SpawnActor<ABullet_Missile>(HomingMissileFactory, firePosition1->GetComponentLocation(), firePosition1->GetComponentRotation() + FRotator(spread_Pitch, spread_Yaw,0), param);
	
	bullet->SetBulletSpeed(HomingMissileSpeed);
	float bulletSpeed = bullet->GetBulletSpeed();
	bullet->SetLifeTime(5, false);	
	bullet->SetRealTimeHomingMissile(target, HomingValue);
	
}