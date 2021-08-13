// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBoss.h"
#include "EnemyBoss_Demon.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API AEnemyBoss_Demon : public AEnemyBoss
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AEnemyBoss_Demon(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = "collComp")
		class UBoxComponent* collComp;
	UPROPERTY(EditDefaultsOnly, Category = "collComp")
		class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, Category = "collComp")
		class UArrowComponent* firePosition1;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
		void FIreWorkHomingMissile();
	UFUNCTION()
		void FireHomingMissile();
		

private:

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TSubclassOf <class ABullet_Missile> HomingMissileFactory;

	UPROPERTY()
		class APlayer_Human* target;
	UPROPERTY()
		float targetDistance;

	// 호밍미사일
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float fireHomingMissileRate = 1;
	UPROPERTY()
		float currentFireHomingMissileRate = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float HomingMissileSpeed = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float HomingValue = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float spread = 5;
	//
	
	
};
