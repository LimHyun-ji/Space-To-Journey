// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_BulletBall.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemy_BulletBall::AEnemy_BulletBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemy_BulletBall::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AEnemy_BulletBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	Move();
	LifeTime();
}

void AEnemy_BulletBall::Move()
{
	dir = GetActorForwardVector();
	dir.Normalize();

	FVector P0 = GetActorLocation();
	FVector vel = speed * GetWorld()->DeltaTimeSeconds * dir;
	FVector P = vel + P0;

	SetActorLocation(P);
}

void AEnemy_BulletBall::OnCollisionEnter_Overlab(AActor* OtherActor)
{

}


