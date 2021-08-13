// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillAttack1.h"
#include <Particles/ParticleSystemComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/SphereComponent.h>

// Sets default values
ASkillAttack1::ASkillAttack1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp=CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent=SphereComp;

	Effect=CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	Effect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASkillAttack1::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySound2D(GetWorld(), crashSound);
}

// Called every frame
void ASkillAttack1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime();
}

float ASkillAttack1::GetAttackDamage()
{
	return attackDamage;
}

void ASkillAttack1::LifeTime()
{
	currentLifeTime+=GetWorld()->DeltaTimeSeconds;
	if(currentLifeTime>lifeTime)
		Destroy();
}

