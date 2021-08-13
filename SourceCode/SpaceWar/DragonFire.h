// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragonFire.generated.h"

UCLASS()
class SPACEWAR_API ADragonFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADragonFire();

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystemComponent* FireEffect;

	UPROPERTY(EditDefaultsOnly, Category="Settings")
		class USoundBase* FireSound;
	UPROPERTY(EditDefaultsOnly, Category="Settings")
		int attackDamage=10;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	float currentLifeTIme=0.0f;

	UPROPERTY()
	float lifeTime=3.0f;

	UPROPERTY(EditDefaultsOnly, Category="Settings")
	float speed=3000;

	UFUNCTION()
	void LifeTime();

	UFUNCTION()
	void Move();

	UFUNCTION()
	int GetAttackDamage();
};
