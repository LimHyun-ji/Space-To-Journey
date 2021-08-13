// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_BulletBasic.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemy_BulletBasic::AEnemy_BulletBasic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy_BulletBasic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_BulletBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	Move();
	LifeTime();
	
}

void AEnemy_BulletBasic::Move()
{
	dir = GetActorForwardVector();
	dir.Normalize();

	FVector P0 = GetActorLocation();
	FVector vel = speed * GetWorld()->DeltaTimeSeconds * dir;
	FVector P = vel + P0;

	SetActorLocation(P);
}



void AEnemy_BulletBasic::OnCollisionEnter_Overlab(AActor* OtherActor)
{

}

