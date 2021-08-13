// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Observer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RecallEnemy.h"
#include "PlayerCPP.h"

// Sets default values
AEnemy_Observer::AEnemy_Observer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	detectComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Detect Area"));
	detectComp->SetupAttachment(collComp);
	detectComp->CreateDynamicMaterialInstance(0);


}

// Called when the game starts or when spawned
void AEnemy_Observer::BeginPlay()
{
	Super::BeginPlay();

	detectComp->SetVectorParameterValueOnMaterials(TEXT("color"), beforeDetectColor);
	detectComp->SetVectorParameterValueOnMaterials(TEXT("emissiveColor"), beforeDetectColor);
}

// Called every frame
void AEnemy_Observer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (!isDetectPlayer && !isRecallDone)
	{
		Turn();
	}
	else if (isDetectPlayer && !isRecallDone)
	{
		RecallEnemy();
	}

}

void AEnemy_Observer::Turn()
{

	if (!isTurn && !isWait)
	{
		turnSpeed_Pitch = FMath::RandRange(-maxTurnSpeed, maxTurnSpeed);
		turnSpeed_Yaw = FMath::RandRange(-maxTurnSpeed, maxTurnSpeed);

		isTurn = true;
	}
	else if (isWait)
	{
		currentWaitTime += GetWorld()->DeltaTimeSeconds;

		if (currentWaitTime > waitTime)
		{
			isWait = false;
			currentWaitTime = 0;
		}
	}
	else
	{
		currentTurnTime += GetWorld()->DeltaTimeSeconds;

		FRotator NewAddRotation = FRotator(turnSpeed_Pitch, turnSpeed_Yaw, 0);

		if (abs(GetActorRotation().Pitch + NewAddRotation.Pitch) > maxPitchRotation)
		{
			NewAddRotation.Pitch = 0;
		}

		SetActorRotation(GetActorRotation() + NewAddRotation);

		if (currentTurnTime > turnTime)
		{
			currentTurnTime = 0;
			isTurn = false;
			isWait = true;
			waitTime = FMath::RandRange(1.0, maxWaitTime);
		}

	}

}

void AEnemy_Observer::RecallEnemy()
{

	for (int i = 0; i < 6; i++)
	{
		FRotator spawnRotation;
		FVector spawnLocation;

		spawnRotation.Yaw = FMath::RandRange(0, 360);
		spawnRotation.Pitch = 0;
		spawnRotation.Roll = 0;

		//spawnLocation.X = (targetLocation.X + FMath::RandRange(-5000, 5000));
		spawnLocation = (targetForward * 18000) + targetLocation;
		spawnLocation.Y += FMath::RandRange(-8000, 8000);
		spawnLocation.Z += FMath::RandRange(-2000, 2000);

		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ARecallEnemy* Recall = GetWorld()->SpawnActor<ARecallEnemy>(RecallEnemyFactory, spawnLocation, spawnRotation, param);

		if (i < 2)
		{
			Recall->SetRecallEnemyType(1);
		}
		else
		{
			Recall->SetRecallEnemyType(0);
		}

		Recall->SetRecallTime(FMath::RandRange(3.0f, 5.0f));

	}


	isRecallDone = true;
}

void AEnemy_Observer::DetectPlayer(AActor* OtherActor)		//감지할 경우가 플래이어만 있으므로 굳이 isvalid를 사용하지 않음
{
	if (!isDetectPlayer)
	{
		APlayerCPP* player = Cast<APlayerCPP>(OtherActor);
		player->SetMessage("Detected by the enemy.");

		detectComp->SetVectorParameterValueOnMaterials(TEXT("color"), afterDetectColor);
		detectComp->SetVectorParameterValueOnMaterials(TEXT("emissiveColor"), afterDetectColor);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), warningSound, GetActorLocation());

		targetLocation = OtherActor->GetActorLocation();
		targetForward = OtherActor->GetActorForwardVector();
		isDetectPlayer = true;
	}
}

