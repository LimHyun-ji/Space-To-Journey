
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceEnemy.generated.h"

UCLASS()
class SPACEWAR_API ASpaceEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpaceEnemy(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* collComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
		class APlayerCPP* removeByThisPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = true))
		uint8 EnemyNumber = 0;

	UPROPERTY(VisibleAnywhere, Category = "Visible Target")
		class APlayerCPP* target;

	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = true))
		TSubclassOf <class AAllBulletParent> bulletFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		class UParticleSystem* exploreFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundBase* deadSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = true))
		class USoundBase* fireSound;

	UPROPERTY()
		float currentHp;
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (AllowPrivateAccess = true))
		float maxHp = 20;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* HpBarWidget;
	UPROPERTY()
		class UEnemyHpBar_Widget* HpBar;

	UPROPERTY()
		bool isVisibleHpBar = false;
	UPROPERTY()
		float currentHpBarOpacity = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = true))
		float HpBarVisualTime = 5;
	UPROPERTY()
		float currentHpBarVisualTime = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = true))
		float opacitySpeed = 0.01;
	UPROPERTY()
		bool isHpBarOpacityWork = false;


	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* targetDisplayWidget;
	UPROPERTY()
		class UTargetDisplayWidget* targetDisplay;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float activateTargetDisplayDistance = 100000;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float ArrowMoveDistance = 10000;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float ArrowMoveSpeed = 100;

	///////Drop Item
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TSubclassOf <class AItem> itemFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		uint8 dropItemNumber;
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (AllowPrivateAccess = true))
		uint8 dropItemPercent = 40;
	///////

	UPROPERTY(EditDefaultsOnly, Category = "stats", meta = (AllowPrivateAccess = true))
		float magazine = 15;
	UPROPERTY()
		float currentMagazine = magazine;
	UPROPERTY(EditDefaultsOnly, Category = "stats", meta = (AllowPrivateAccess = true))
		float reloadingDelay = 3;
	UPROPERTY()
		float currentReloadingDelay = 0;
	UPROPERTY()
		bool isReloading = false;
	UPROPERTY(EditDefaultsOnly, Category = "stats", meta = (AllowPrivateAccess = true))
		bool isInfinityMagazine = false;

	UPROPERTY(EditDefaultsOnly, Category = "stats", meta = (AllowPrivateAccess = true))
		float fireRate = 0.2;
	UPROPERTY()
		float currentFireRate = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float bulletSpeed = 10000;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		float GetCurrentHp();
	UFUNCTION()
		float GetMaxHp();

	UFUNCTION()
		void SetHpBarVisible(bool OffOn);	//0 : 안보임	1 : 보임
	UFUNCTION()
		void HpBarOpacityWork();
	UFUNCTION()
		void SetHpBarOpacityWork();


	UFUNCTION()
		void ActivateTargetDisplay();
	UFUNCTION()
		void MoveTargetDisplay();

	UFUNCTION()
		void Reloading();
	UFUNCTION()
		void FireWork();

	virtual void Fire() { ; }	//메모리를 미리 잡아 놓지 않는듯. 그래서 UFUNCTION을 쓰면 안되나봄. 썼다가 오류로 고생함

	///////Drop Item

	UFUNCTION()
		void DropItem();
	UFUNCTION()
		void TakeDamage(float inputDamage);
	UFUNCTION()
		void SetRemoveByThisPlayer(APlayerCPP* inputRemoveByThisPlayer);
};
