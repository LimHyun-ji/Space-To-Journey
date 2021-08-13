// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager_FA.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy_FarAway.h"
#include <Components/BoxComponent.h>

// Sets default values
ASpawnManager_FA::ASpawnManager_FA()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoundBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundBox"));
	RootComponent = BoundBox;
}

// Called when the game starts or when spawned
void ASpawnManager_FA::BeginPlay()
{
	countDownTemp = spawnCountDown;
	Super::BeginPlay();
	for (int i = 0; i < spawnAmount; i++)
	{
		SpawnEnemy();
	}
	//find spawnManager
	TArray <AActor*> obj;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy_FarAway::StaticClass(), obj);
	for (AActor* actor : obj)
	{
		EnemyFind = Cast<AEnemy_FarAway>(actor);
	}
}

// Called every frame
void ASpawnManager_FA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyFind == nullptr)
	{
		countDownTemp -= GetWorld()->DeltaTimeSeconds;

		if (countDownTemp < 0)
		{
			for (int i = 0; i < spawnAmount; i++)
			{
				SpawnEnemy();
			}
			//find spawnManager
			TArray <AActor*> obj;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy_FarAway::StaticClass(), obj);
			for (AActor* actor : obj)
			{
				EnemyFind = Cast<AEnemy_FarAway>(actor);
			}
			countDownTemp = spawnCountDown;
		}
	}


}

void ASpawnManager_FA::SpawnEnemy()
{
	FVector boxSize = BoundBox->GetScaledBoxExtent();
	//GetActorBounds(true, boundBoxLocation, boxSize);
	//Box의 Bound안에서 랜덤으로 값 설정

	InitRotation.Yaw = FMath::RandRange(-180, 180);
	InitLocation.X = FMath::RandRange(GetActorLocation().X - boxSize.X, GetActorLocation().X + boxSize.X);
	InitLocation.Y = FMath::RandRange(GetActorLocation().Y - boxSize.Y, GetActorLocation().Y + boxSize.Y);
	InitLocation.Z = GetActorLocation().Z - boxSize.Z;
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AEnemy_FarAway>(Enemy, InitLocation, InitRotation, param);

}

