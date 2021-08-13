// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecallEnemy.generated.h"

UCLASS()
class SPACEWAR_API ARecallEnemy : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* collComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;
	
	

public:	
	// Sets default values for this actor's properties
	ARecallEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Grow();
	UFUNCTION()
		void RecallEnemy();
	UFUNCTION()
		void SetRecallTime(float time);
	UFUNCTION()
		void SetMaxScale();
	UFUNCTION()
		void SetRecallEnemyType(uint8 type);
	UFUNCTION()
		void Decrease();
	UFUNCTION()
		void Spin();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetRecallDone();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		class USoundBase* recallSound;

	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = true))
		TSubclassOf <class AEnemy_Hunter> Enemy_HunterFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = true))
		TSubclassOf <class AEnemy_SpaceGalleon> Enemy_SpaceGalleonFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = true))
		float recallTime = 1;
	UPROPERTY()
		float currentRecallTime = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = true))
		float maxScale = 3;
	UPROPERTY()
		float growSpeed;
	UPROPERTY()
		float decreaseSpeed;
	UPROPERTY()
		bool isRecallDone;
	UPROPERTY()
		bool isSetting = false;

	UPROPERTY(EditAnywhere, Category = "Setting", meta = (AllowPrivateAccess = true))
		uint8 spawnEnemyType = 0;	// 0 : ÇåÅÍ   1 : °¶¸®¿Â

};
