// Fill out your copyright notice in the Description page of Project Settings.


#include "H_PlayerBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include <Particles/ParticleSystemComponent.h>

// Sets default values
AH_PlayerBullet::AH_PlayerBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	LaunchEffect=CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaunchEffect"));
	LaunchEffect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AH_PlayerBullet::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
}

// Called every frame
void AH_PlayerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
	LifeTime();
}

void AH_PlayerBullet::Move()
{
	FVector dir = GetActorForwardVector();
	dir.Normalize();
	FVector newLocation;
	newLocation = GetActorLocation() + dir * speed * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(newLocation);
}

void AH_PlayerBullet::LifeTime()
{
	currentLifeTime += GetWorld()->DeltaTimeSeconds;
	if (currentLifeTime > lifeTime)
		Destroy();
}

int AH_PlayerBullet::GetBulletDamage()
{
	return bulletDamage;
}

void AH_PlayerBullet::ExploreEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploreFactory, GetActorLocation(), GetActorRotation(), GetActorScale3D() - FVector(0.3, 0.3, 0.3));

}

void AH_PlayerBullet::ExploreEffect_Dragon()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploreFactory_Dragon, GetActorLocation(), GetActorRotation(), GetActorScale3D() - FVector(0.3, 0.3, 0.3));

}

