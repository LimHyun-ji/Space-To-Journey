// Fill out your copyright notice in the Description page of Project Settings.


#include "RecallEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy_SpaceGalleon.h"
#include "Enemy_Hunter.h"


// Sets default values
ARecallEnemy::ARecallEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = collComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ARecallEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), recallSound,GetActorLocation());
}

// Called every frame
void ARecallEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isSetting)
	{
		SetMaxScale();
		SetActorScale3D(FVector(0, 0, 0));
		growSpeed = (maxScale / recallTime);
		decreaseSpeed = (maxScale / 0.1);
		isSetting = true;
	}
	
	currentRecallTime += DeltaTime;

	if (currentRecallTime > recallTime && !isRecallDone)
	{
		RecallEnemy();		
		isRecallDone = true;
	}

	if (isRecallDone)
	{
		Decrease();
		if (GetActorScale3D().X < 0.1)
		{
			Destroy();
		}
	}

	Grow();

	Spin();
}

void ARecallEnemy::Grow()
{
	
	SetActorScale3D(GetActorScale3D() + FVector(growSpeed*GetWorld()->DeltaTimeSeconds, growSpeed * GetWorld()->DeltaTimeSeconds, growSpeed * GetWorld()->DeltaTimeSeconds));
}

void ARecallEnemy::RecallEnemy()
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector spawnLocation = GetActorLocation();
	FRotator spawnRotation = GetActorRotation();
	spawnRotation.Pitch = 0;
	spawnRotation.Roll = 0;

	if (spawnEnemyType == 0)
	{
		GetWorld()->SpawnActor<AEnemy_Hunter>(Enemy_HunterFactory, spawnLocation, spawnRotation, param);
	}
	else if (spawnEnemyType == 1)
	{
		GetWorld()->SpawnActor<AEnemy_SpaceGalleon>(Enemy_SpaceGalleonFactory, spawnLocation, spawnRotation, param);
	}
}

void ARecallEnemy::SetRecallTime(float time)
{
	recallTime = time;
}

void ARecallEnemy::SetMaxScale()
{
	switch (spawnEnemyType)
	{
	case 0:
		maxScale = 1;
		break;
	case 1:
		maxScale = 6;
		break;
	}
}

void ARecallEnemy::SetRecallEnemyType(uint8 type)
{
	spawnEnemyType = type;
}

void ARecallEnemy::Decrease()
{
	SetActorScale3D(GetActorScale3D() - FVector(decreaseSpeed * GetWorld()->DeltaTimeSeconds, decreaseSpeed * GetWorld()->DeltaTimeSeconds, decreaseSpeed * GetWorld()->DeltaTimeSeconds));
}

void ARecallEnemy::Spin()
{
	SetActorRotation(GetActorRotation() + FRotator(0, 0, 50));
}

bool ARecallEnemy::GetRecallDone()
{
	return isRecallDone;
}