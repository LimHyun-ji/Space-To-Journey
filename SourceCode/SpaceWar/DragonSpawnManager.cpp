// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonSpawnManager.h"
#include <Components/ArrowComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Enemy_Dragon.h"
#include <Components/BoxComponent.h>
#include "Player_Human.h"

// Sets default values
ADragonSpawnManager::ADragonSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
// 
// 	class USceneComponent* SceneComponent=CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
// 	RootComponent=SceneComponent;
	
	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent=BoxComp;

	VehicleSpawnPosition=CreateDefaultSubobject<UArrowComponent>(TEXT("VehicleSpawnPosition"));
	VehicleSpawnPosition->SetupAttachment(RootComponent);

	EnemySpawnPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("EnemySpawnPosition"));
	EnemySpawnPosition->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADragonSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	TArray <AActor*> obj2;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Human::StaticClass(), obj2);
	for (AActor* actor : obj2)
	{
		target = Cast<APlayer_Human>(actor);
		if (IsValid(target))
			break;
	}
}

// Called every frame
void ADragonSpawnManager::Tick(float DeltaTime)
{

	if (Dragon == nullptr)
	{
		TArray <AActor*> obj2;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy_Dragon::StaticClass(), obj2);
		for (AActor* actor : obj2)
		{
			Dragon = Cast<AEnemy_Dragon>(actor);
			if (IsValid(Dragon))
				break;
		}
	}

	Super::Tick(DeltaTime);
	if (!target->isCatchDragon && !isSpawnEnemy && !isSpawnVehicle)
	{
		FActorSpawnParameters params;
		params.Owner = this;
		params.Instigator = GetInstigator();

		AEnemy_Dragon* EnemyDragon = GetWorld()->SpawnActor<AEnemy_Dragon>(DragonEnemyClass, EnemySpawnPosition->GetComponentTransform(), params);
		isSpawnEnemy=true;
	}
	else if(target->isCatchDragon && !isSpawnVehicle && !isSpawnEnemy)
	{
		FActorSpawnParameters params;
		params.Owner = this;
		params.Instigator = GetInstigator();

		AVehicle_Dragon* VehicleDragon = GetWorld()->SpawnActor<AVehicle_Dragon>(DragonVehicleClass, VehicleSpawnPosition->GetComponentTransform(), params);
		isSpawnVehicle=true;
	}


	if (Dragon != nullptr && Dragon->isDead && !canStartTimer)
	{
		canStartTimer=true;
	}
	
	if (canStartTimer && !isSpawnVehicle && DragonVehicleClass)
	{
		currentSpawnDelay += GetWorld()->DeltaTimeSeconds;
		if (currentSpawnDelay > spawnDelay)
		{
			FActorSpawnParameters params;
			params.Owner = this;
			params.Instigator = GetInstigator();

			AVehicle_Dragon* VehicleDragon = GetWorld()->SpawnActor<AVehicle_Dragon>(DragonVehicleClass, VehicleSpawnPosition->GetComponentTransform(), params);
			UE_LOG(LogTemp, Warning, TEXT("Spawn"));
			isSpawnVehicle = true;
		}
	}
}

