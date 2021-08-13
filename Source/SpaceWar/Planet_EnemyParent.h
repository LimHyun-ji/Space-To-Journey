// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Planet_EnemyParent.generated.h"

UCLASS()
class SPACEWAR_API APlanet_EnemyParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanet_EnemyParent();

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

	UPROPERTY()
		class APlayer_Human* target;
	UPROPERTY()
		class AH_PlayerBullet* playerBullet;

	UPROPERTY()
		class ASkillAttack1* skillAttack1;

	UPROPERTY()
		FVector InitLocation;
	UPROPERTY()
		FRotator runAwayGoalRotation;
	UPROPERTY()
		FRotator returnGoalRotation;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float speed = 100;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int enemyHP = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int attackDamage = 10;
	UPROPERTY()
		bool isWalk = false;
	UPROPERTY()
		bool isAttack = false;
	UPROPERTY()
		bool isHit = false;
	UPROPERTY()
		bool isDead = false;
	UPROPERTY()
		bool isMovingBackward=false;
	UPROPERTY()
		float deathCountDown = 4.0f;
	UPROPERTY()
		float currentAttackDelay=0;
	UPROPERTY(EditDefaultsOnly, Category="Settings")
		float attackDelay=1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TSubclassOf <class AItem> itemFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		uint8 dropItemNumber;
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (AllowPrivateAccess = true))
		uint8 dropItemPercent = 40;

	UFUNCTION()
		void Attack();
	UFUNCTION()
		void Dead();
	UFUNCTION()
		void DropItem();
	UFUNCTION()
		void ReturnMove();
	UFUNCTION()
		void AttackMove();
	UFUNCTION()
		void RunAway();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnOverlapBegin(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnOverlapEnd(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsWalk();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsHit();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsAttack();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsDead();
	UFUNCTION()
		int GetAttackDamage();

	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsMovingBackward();

};
