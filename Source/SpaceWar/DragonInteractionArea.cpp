// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonInteractionArea.h"
#include <Kismet/GameplayStatics.h>
#include "Player_Human.h"
#include "Enemy_Dragon.h"
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <Particles/ParticleSystemComponent.h>


// Sets default values
ADragonInteractionArea::ADragonInteractionArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent=BoxComp;

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	EffectPosition=CreateDefaultSubobject<UArrowComponent>(TEXT("EffectPosition"));
	EffectPosition->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADragonInteractionArea::BeginPlay()
{
	Super::BeginPlay();

	TArray <AActor*> obj1;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Human::StaticClass(), obj1);
	for (AActor* actor : obj1)
	{
		target = Cast<APlayer_Human>(actor);
		if (IsValid(target))
			break;
	}
}

// Called every frame
void ADragonInteractionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (dragon == nullptr)
	{
		TArray <AActor*> obj2;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy_Dragon::StaticClass(), obj2);
		for (AActor* actor : obj2)
		{
			dragon = Cast<AEnemy_Dragon>(actor);
			if (IsValid(dragon))
				break;
		}
	}
	

	if (!canInteract)
	{
		if (!isSpawnEmitter)
		{
			fireParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), openKeyEffect, EffectPosition->GetComponentLocation(), EffectPosition->GetComponentRotation(), GetActorScale3D() - FVector(0.3, 0.3, 0.3));
			isSpawnEmitter = true;
			target->InteractionWidgetRemoveAdd(0);
			UE_LOG(LogTemp, Warning, TEXT("Spawn %d"), orderNum);
		}
	}
	if (target->order == "")
	{
		canInteract=true;
		//spawn remove
		if(fireParticle)
			fireParticle->DestroyComponent();
		isSpawnEmitter=false;
	}
	if (wakeUpDragon && dragon)
	{
		dragon->SetIsInteraction(true);
		dragon->SetIsWakeUp(true);
		dragon->SetIsCineCamera(true);
		wakeUpDragon=false;//한 번만 깨우기 위해
	}
}

///////////InteractionAreaEvent/////////////////
void ADragonInteractionArea::BeginOverlapInteraction(AActor* OtherActor)
{
	if (OtherActor != this && target != nullptr && dragon != nullptr)
	{
// 		if (OtherActor == target)
// 			dragon->SetIsInteraction(true);
// 			dragon->SetIsWakeUp(true);
	}
}

void ADragonInteractionArea::EndOverlapInteraction(AActor* OtherActor)
{
	if (OtherActor != this && target != nullptr && dragon != nullptr)
	{
// 		if (OtherActor == target)
// 			dragon->SetIsInteraction(false);
// 			dragon->SetIsWakeUp(false);
	}
}