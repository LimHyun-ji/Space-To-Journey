// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy_FarAway.generated.h"

UCLASS()
class SPACEWAR_API AEnemy_FarAway : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy_FarAway();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		class UCapsuleComponent* CapComp;

	UPROPERTY(EditDefaultsOnly)
		class USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Visible Target")
		class APlayer_Human* target;
	UPROPERTY(VisibleAnywhere, Category = "Visible Target")
		class AH_PlayerBullet* playerBullet;

	UPROPERTY(VisibleAnywhere, Category = "Visible Target")
		class ASkillAttack1* skillAttack1;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TSubclassOf <class AEnemy_FarAwayAttack> attack;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float speed = 100;
	UPROPERTY()
		float spawnCountDown = 1.0f;
	UPROPERTY()
		FRotator runAwayGoalRotation;
	UPROPERTY()
		FRotator returnGoalRotation;
	UPROPERTY()
		FVector InitLocation;
	UPROPERTY()
		bool isAttack = false;
	UPROPERTY()
		bool isHit = false;
	UPROPERTY()
		bool isDead = false;
	UPROPERTY()
		bool isMovingBackward = false;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int enemyHP = 10;
	UPROPERTY()
		float deathCountDown = 4.0f;

	UFUNCTION()
		void RunAway(float dletaTime);

	UFUNCTION()
		void ReturnMove(float deltaTIme);

	UFUNCTION()
		void AttackMove();
	UFUNCTION()
		void Attack();


	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsHit();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsAttack();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsDead();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsMovingBackward();

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnOverlapBegin(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnOverlapEnd(AActor* OtherActor);

};
