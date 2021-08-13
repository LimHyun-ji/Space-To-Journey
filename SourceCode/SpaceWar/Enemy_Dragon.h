// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy_Dragon.generated.h"

UCLASS()
class SPACEWAR_API AEnemy_Dragon : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy_Dragon();

	UPROPERTY(EditDefaultsOnly)
		class UCapsuleComponent* PhysicsComp;

	UPROPERTY(EditDefaultsOnly)
		class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
		class UArrowComponent* FirePosition;

	UPROPERTY(EditDefaultsOnly)
		class UArrowComponent* BitePosition;

	UPROPERTY(EditDefaultsOnly)
		class UArrowComponent* ClawPosition1;

	UPROPERTY(EditDefaultsOnly)
		class UArrowComponent* ClawPosition2;

	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* AttackInteraction;
	UPROPERTY(EditDefaultsOnly, Category="Settings")
		TSubclassOf<class AEnemy_BulletBall> ClawClass;

	UPROPERTY()//player Camera
	FVector playerCameraLocation;
	UPROPERTY()
	FRotator playerCameraRotation;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		class APlayer_Human* target;


	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class ADragonFire> SpitClass;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class ADragonFire> SpreadClass;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		class UParticleSystem* BiteEffect;
	UPROPERTY(EditDefaultsOnly)
		float speed = 2000;
	UPROPERTY()
		bool isFlying;
	UPROPERTY()
		bool isInteracting;
	UPROPERTY()
		bool isSpitting;
	UPROPERTY()
		bool isSpreading;
	UPROPERTY()
		bool isWakeUp;
	UPROPERTY()
		bool isAttackActivate = false;
	UPROPERTY()
		bool isGliding=false;
	UPROPERTY()
		bool isHit;
	UPROPERTY()
		float spitDelayTime = 1.0f;
	UPROPERTY()
		float currentSpitDelayTime = 0.0f;
	UPROPERTY()
		FVector InitLocation;
	UPROPERTY()
		FRotator InitRotation;
	UPROPERTY()
		FRotator glidingStartRotation;
	UPROPERTY()
	FVector targetPrevLocation;
	UPROPERTY()
		int attackMode=0;
	UPROPERTY()
		int flyingPattern = 0;
	UPROPERTY(EditDefaultsOnly, Category="Settings")
		float wakeUpDelay = 1.0f;
	UPROPERTY()
		float currentWakeUpDelay = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float patternTime = 7.0f;
	UPROPERTY()
		float currentPatternTime = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float attackTime = 5.0f;
	UPROPERTY()
		float currentAttackTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float deathDelayTime = 4.0f;
	UPROPERTY()
		float currentDeathDelayTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float clawAttackTime = 2.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float clawSpawnTime = 0.0f;
	UPROPERTY()
		bool isClawSpawn=false;
	UPROPERTY()
		float currentClawAttackTime = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float biteSpawnTime = 1.0f;
	UPROPERTY()
		float currentBiteSpawnTime = 0.0f;
	UPROPERTY()
		bool isDead=false;
	UPROPERTY()
		bool isResetPitch=false;
	UPROPERTY()
		bool isWalking=false;
	UPROPERTY()
		bool isBiteAttack=false;
	UPROPERTY()
		bool isClawAttack=false;
	UPROPERTY()
		int patternMode=0;
	UPROPERTY(EditDefaultsOnly, Category="Settings")
		int dragonHP=10;
	UPROPERTY(EditDefaultsOnly, Category="Settings")
		int biteAttackDamage=10;
	UPROPERTY()
		float currentCineCameraDelay=0.0f;
	UPROPERTY()
		float cineCameraDelay=4.0f;
	UPROPERTY()
		bool isCineCamera=false;
	UFUNCTION()
		void WakeUp();
	UFUNCTION()
		void SpitFireBall();
	UFUNCTION()
		void SpreadFire();
	UFUNCTION()
		void ReturnToInit();
	UFUNCTION()
		void Gliding();
	UFUNCTION()
		void Grounding();
	UFUNCTION()
		void ResetSettings();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsFlying();

	UFUNCTION()
		void SetIsInteraction(bool input);
	UFUNCTION()
		void SetIsWakeUp(bool input);
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsSpitting();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsSpreading();
	UFUNCTION(BluePrintCallable, Category="Code")
		bool GetIsHit();
	UFUNCTION(BluePrintCallable, Category = "Code")
		bool GetIsWalking();
	UFUNCTION(BluePrintCallable, Category = "Code")
		bool GetIsBite();
	UFUNCTION(BluePrintCallable, Category = "Code")
		bool GetIsGliding();
	UFUNCTION(BluePrintCallable, Category = "Code")
		bool GetIsDead();
	UFUNCTION(BluePrintCallable, Category = "Code")
		bool GetIsClaw();
	UFUNCTION()
		int GetBiteAttackDamage();
	UFUNCTION(BlueprintCallable, Category="Code")
		bool GetIsCineCamera();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void SetIsCineCamera(bool input);
	//////////Event//////////////
	UFUNCTION(BlueprintCallable, Category="Code")
		void OnHitOverlap_Begin(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnHitOverlap_End(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnAttackOverlap_Begin(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnAttackOverlap_End(AActor* OtherActor);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
