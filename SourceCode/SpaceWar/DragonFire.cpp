// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonFire.h"
#include <Components/SphereComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ADragonFire::ADragonFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp=CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent=CollisionComp;

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	FireEffect=CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
	FireEffect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADragonFire::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySound2D(GetWorld(), FireSound);
}

// Called every frame
void ADragonFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime();
	Move();
}

void ADragonFire::LifeTime()
{
	currentLifeTIme+=GetWorld()->DeltaTimeSeconds;
	if(currentLifeTIme>lifeTime)
		Destroy();
}

void ADragonFire::Move()
{
	FVector newLocation;
	FVector dir=GetActorForwardVector();
	newLocation=GetActorLocation()+ dir*speed*GetWorld()->DeltaTimeSeconds;

	SetActorLocation(newLocation);
}

int ADragonFire::GetAttackDamage()
{
	return attackDamage;
}

