// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_FirstBorn.h"
#include "PlayerCPP.h"
#include "Player_Human.h"

ANPC_FirstBorn::ANPC_FirstBorn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPC_FirstBorn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ANPC_FirstBorn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

