// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillAttack1.generated.h"

UCLASS()
class SPACEWAR_API ASkillAttack1 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillAttack1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystemComponent* Effect;

	UPROPERTY(EditDefaultsOnly, Category = "Bgm")
		class USoundBase* crashSound;

	UPROPERTY(EditDefaultsOnly)
	 float attackDamage=30;
	UPROPERTY()
	float currentLifeTime=0.0f;
	UPROPERTY()
	float lifeTime=2.0f;

	UFUNCTION()
	float GetAttackDamage();

	UFUNCTION()
	void LifeTime();
};
