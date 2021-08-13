// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include <Kismet/GameplayStatics.h>
#include "Planet_EnemyParent.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoundBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundBox"));
	RootComponent = BoundBox;
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	countDownTemp = spawnCountDown;
	Super::BeginPlay();
	for (int i = 0; i < spawnAmount; i++)
	{
		SpawnEnemy();
	}
	//find spawnManager
	TArray <AActor*> obj;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet_EnemyParent::StaticClass(), obj);
	for (AActor* actor : obj)
	{
		EnemyFind= Cast<APlanet_EnemyParent>(actor);
	}
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
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
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet_EnemyParent::StaticClass(), obj);
			for (AActor* actor : obj)
			{
				EnemyFind = Cast<APlanet_EnemyParent>(actor);
			}
			countDownTemp = spawnCountDown;
		}
	}


}

void ASpawnManager::SpawnEnemy()
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
	GetWorld()->SpawnActor<APlanet_EnemyParent>(EnemyFactory, InitLocation, InitRotation, param);

}

