// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet_Parent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEnemyBullet_Parent::AEnemyBullet_Parent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = collComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AEnemyBullet_Parent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBullet_Parent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBullet_Parent::LifeTime()
{
	currentLifeTime += GetWorld()->DeltaTimeSeconds;

	if (currentLifeTime > lifeTime)
	{
		Destroy();
	}
}

float AEnemyBullet_Parent::GetBulletDamage()
{
	return bulletDamage;
}

void AEnemyBullet_Parent::ExploreEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploreFactory, GetActorLocation(), GetActorRotation(), GetActorScale3D());
}
