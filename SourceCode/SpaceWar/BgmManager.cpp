// Fill out your copyright notice in the Description page of Project Settings.


#include "BgmManager.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABgmManager::ABgmManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABgmManager::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::PlaySound2D(GetWorld(), BGM1);	

}

// Called every frame
void ABgmManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

