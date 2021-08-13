// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceEnemy.h"
#include "Enemy_Observer.generated.h"

UCLASS()
class SPACEWAR_API AEnemy_Observer : public ASpaceEnemy
{
	GENERATED_BODY()



public:
	// Sets default values for this actor's properties
	AEnemy_Observer(const FObjectInitializer& ObjectInitializer);



	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* detectComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Turn();


	UFUNCTION()
		void RecallEnemy();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void DetectPlayer(AActor* OtherActor);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = true))
		TSubclassOf <class ARecallEnemy> RecallEnemyFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		class USoundBase* warningSound;

	UPROPERTY(EditDefaultsOnly, Category = "Component", meta = (AllowPrivateAccess = true))
		float maxTurnSpeed = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Component", meta = (AllowPrivateAccess = true))
		float maxPitchRotation = 40;
	UPROPERTY()
		float turnSpeed_Pitch;
	UPROPERTY()
		float turnSpeed_Yaw;
	UPROPERTY()
		float turnTime = 1;
	UPROPERTY()
		float currentTurnTime = 0;
	UPROPERTY()
		bool isTurn = false;

	UPROPERTY()
		float maxWaitTime = 5;
	UPROPERTY()
		float waitTime = 5;
	UPROPERTY()
		float currentWaitTime = 0;
	UPROPERTY()
		bool isWait = false;

	UPROPERTY()
		bool isDetectPlayer = false;
	UPROPERTY()
		bool isRecallDone = false;

	UPROPERTY()
		FVector targetLocation;
	UPROPERTY()
		FVector targetForward;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		FVector beforeDetectColor;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		FVector afterDetectColor;
};
