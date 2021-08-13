// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "H_PlayerBullet.generated.h"

UCLASS()
class SPACEWAR_API AH_PlayerBullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AH_PlayerBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* BoxComp;
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		class UParticleSystem* exploreFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		class UParticleSystem* exploreFactory_Dragon;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		class UParticleSystemComponent* LaunchEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Bgm")
		class USoundBase* fireSound;

	UPROPERTY()
		float currentLifeTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float lifeTime = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float speed = 1000;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float bulletDamage = 5;



	UFUNCTION()
		void Move();
	UFUNCTION()
		void LifeTime();
	UFUNCTION()
		int GetBulletDamage();
	UFUNCTION()
		void ExploreEffect();
	UFUNCTION()
		void ExploreEffect_Dragon();
};
