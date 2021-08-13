// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_ThrowingBall.h"
#include <Components/SphereComponent.h>


//Enemy가 possess해서 손에서 쥐고 있다가 발사하게 하는 방법은 없나
// Sets default values
AEnemy_ThrowingBall::AEnemy_ThrowingBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void AEnemy_ThrowingBall::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy_ThrowingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();
	LifeTime();
}

void AEnemy_ThrowingBall::LifeTime()
{
	currentLifeTime += GetWorld()->DeltaTimeSeconds;
	if (currentLifeTime > lifeTime)
	{
		Destroy();
	}

}

int AEnemy_ThrowingBall::GetAttackDamage()
{
	return attackDamage;
}

void AEnemy_ThrowingBall::Move()
{
	FVector dir = GetActorForwardVector();
	dir.Normalize();
	FVector newLocation;
	newLocation = GetActorLocation() + dir * speed * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(newLocation);
}

