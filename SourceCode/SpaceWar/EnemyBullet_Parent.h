// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllBulletParent.h"
#include "EnemyBullet_Parent.generated.h"

UCLASS()
class SPACEWAR_API AEnemyBullet_Parent : public AAllBulletParent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBullet_Parent();


	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* collComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		class UParticleSystem* exploreFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float speed = 1000;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float lifeTime = 4;
	UPROPERTY()
		float currentLifeTime = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float bulletDamage = 5;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void LifeTime();
	UFUNCTION()
		float GetBulletDamage();
	UFUNCTION()
		void ExploreEffect();
};
