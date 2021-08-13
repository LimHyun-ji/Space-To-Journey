// Fill out your copyright notice in the Description page of Project Settings.


#include "AllEnemyParent.h"

// Sets default values
AAllEnemyParent::AAllEnemyParent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAllEnemyParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAllEnemyParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

