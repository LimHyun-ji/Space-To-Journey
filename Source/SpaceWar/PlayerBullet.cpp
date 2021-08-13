// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Components/ArrowComponent.h>
#include "Item.h"
#include "PlayerCPP.h"

// Sets default values
APlayerBullet::APlayerBullet()
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
void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
}

// Called every frame
void APlayerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();
	LifeTime();

}

void APlayerBullet::Move()
{
	dir = GetActorForwardVector();
	dir.Normalize();

	FVector P0 = GetActorLocation();
	FVector vel = speed * GetWorld()->DeltaTimeSeconds * dir;
	FVector P = vel + P0;

	SetActorLocation(P);
}

void APlayerBullet::LifeTime()
{
	currentLifeTime += GetWorld()->DeltaTimeSeconds;

	if (currentLifeTime > lifeTime)
	{
		Destroy();
	}
}

void APlayerBullet::OnCollisionEnter_Overlab(AActor* OtherActor)
{

}

float APlayerBullet::GetBulletDamage()
{
	return bulletDamage;
}

void APlayerBullet::ExploreEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploreFactory, GetActorLocation(), GetActorRotation(), GetActorScale3D() - FVector(0.3,0.3,0.3));
}

void APlayerBullet::SetBulletDamage(float inputDamage)
{
	bulletDamage = inputDamage;
}

void APlayerBullet::SetBulletOwner(APlayerCPP* inputBulletOwner)
{
	bulletOwner = inputBulletOwner;
}

APlayerCPP* APlayerBullet::GetBulletOwner()
{
	return bulletOwner;
}