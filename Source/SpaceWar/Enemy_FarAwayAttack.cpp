// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_FarAwayAttack.h"
#include <Components/BoxComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include "Player_Human.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemy_FarAwayAttack::AEnemy_FarAwayAttack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MagicParticle"));
	Particle->SetupAttachment(BoxComp);
}

// Called when the game starts or when spawned
void AEnemy_FarAwayAttack::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy_FarAwayAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime();

}

void AEnemy_FarAwayAttack::LifeTime()
{
	currentLifeTime += GetWorld()->DeltaTimeSeconds;
	if (currentLifeTime > lifeTime)
		Destroy();
}

int AEnemy_FarAwayAttack::GetAttackDamage()
{
	return attackDamage;
}

