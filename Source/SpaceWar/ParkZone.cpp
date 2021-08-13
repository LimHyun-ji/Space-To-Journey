// Fill out your copyright notice in the Description page of Project Settings.


#include "ParkZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Components/ArrowComponent.h>

// Sets default values
AParkZone::AParkZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = collComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	parkPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Park Position"));
	parkPosition->SetupAttachment(collComp);
}

// Called when the game starts or when spawned
void AParkZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParkZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


