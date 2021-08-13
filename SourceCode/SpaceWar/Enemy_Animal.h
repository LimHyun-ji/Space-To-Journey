// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Engine/EngineTypes.h>
#include "Player_Human.h"
#include "PlayerBullet.h"
#include "Enemy_Animal.generated.h"

UCLASS()
class SPACEWAR_API AEnemy_Animal : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy_Animal();

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UCapsuleComponent* collComp;
	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* attackInteraction;
	UPROPERTY(EditAnywhere, Category = "Component")
		class USkeletalMeshComponent* meshComp;

	UPROPERTY(VisibleAnywhere, Category = "Visible Target")
		class APlayer_Human* target;
	UPROPERTY(VisibleAnywhere, Category = "Visible Target")
		class AH_PlayerBullet* playerBullet;
	UPROPERTY(VisibleAnywhere, Category = "Visible Target")
		class ASkillAttack1* skillAttack1;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		class ASpawnManager* animalFactory;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float speed = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Status")
		int attackDamage = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Status")
		int animalHP = 10;
	UPROPERTY()
		float deathCountDown = 4.0f;
	UPROPERTY()
		float currentAttackDelay = 0.0f;
	UPROPERTY(EditDefaultsOnly)
		float attackDelay = 1.0f;
	

	UPROPERTY()
		bool isAttackActivation;
	UPROPERTY()
		bool isHit;
	UPROPERTY()
		bool isDead = false;
	UPROPERTY()
		bool setRotating = false;
	UPROPERTY()
		bool isTracking = false;
	UPROPERTY()
		FRotator randomGoalRotation;
	UPROPERTY()
		FRotator returnGoalRotation;
	UPROPERTY()
		FVector InitLocation;




	UFUNCTION()
		void MoveToTarget(float deltaTime);
	UFUNCTION()
		void RandomMove();
	UFUNCTION()
		void ReturnMove(float deltaTime);
	// 	UFUNCTION()
	// 		void SetInitialLocation();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnAttackOverlap_Begin(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnAttackOverlap_End(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnOverlapBegin(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnOverlapEnd(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsAttackActivation();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void SetIsAttackActivation();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsHit();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void SetIsHit();
	UFUNCTION()
		int GetAttackDamage();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsDead();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsTracking();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//≈∏¿Ã∏”

// 	UFUNCTION()
// 	void Timer();
// 	UPROPERTY()
// 	FTimerHandle CountdownTimerHandle;
};
