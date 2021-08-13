// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBox.h"
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Player_Human.h"

// Sets default values
ABulletBox::ABulletBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABulletBox::BeginPlay()
{
	Super::BeginPlay();

	// 	TArray<AActor*> obj;
	// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Human::StaticClass(), obj);
	// 	for (AActor* actor : obj)
	// 	{
	// 		target=Cast<APlayer_Human>(actor);
	// 		if (IsValid(target))
	// 			break;
	// 	}
}

// Called every frame
void ABulletBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletBox::OnCollision(AActor* OtherActor)
{
	// 	if (OtherActor != this && target != nullptr)
	// 	{
	// 		if (OtherActor == target)
	// 		{
	// 			
	// 		}
	// 	}
}

