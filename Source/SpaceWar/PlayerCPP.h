// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Player_Human.h"
#include "PlayerCPP.generated.h"



UCLASS()
class SPACEWAR_API APlayerCPP : public APawn
{
	GENERATED_BODY()

public:

	APlayerCPP(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* collComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class USpringArmComponent* springArmComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UCameraComponent* cameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* firePosition1;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* firePosition2;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* fireMissilePosition1; 
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* fireMissilePosition2;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* player_HumanGetOffPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* player_HumanChairPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* myAimingPoint;

public:
	// Sets default values for this pawn's properties


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		class UParticleSystem* exploreFactory;
	UPROPERTY(VisibleAnywhere, Category = "Player_Human")
		class APlayer_Human* player_Human;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget> underAttackWarningFactory;
	UPROPERTY()
		class UUserWidget* underAttackWarning;

	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* pickUpItem;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* missileSound;

	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_ICant;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_BoardAShip;
	//
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_ImHit1;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_ImHit2;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_ImHit3;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_RemoveEnemy1;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_RemoveEnemy2;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_RemoveEnemy3;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_Fire1;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_Fire2;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_Fire3;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_Dead;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		class USoundBase* voice_Upgrade;
	
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		float voice_ImHitProbability = 20;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		float voice_RemoveEnemyProbability = 20;
	UPROPERTY(EditDefaultsOnly, Category = "SoundSetting")
		float voice_FireProbability = 20;
	//
	/*
	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* PlayerHpBarWidget;
	UPROPERTY()
		class UPlayerHpBar_Widget* PlayerHpBar;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* InventoryWidgetComp;
	UPROPERTY()
		class UInventory_Widget* InventoryWidget;
		*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget>  PlayerHpBarWidget;
	UPROPERTY()
		class UPlayerHpBar_Widget* PlayerHpBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget>  InventoryWidgetComp;
	UPROPERTY()
		class UInventory_Widget* InventoryWidget;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* aimingPointWidgetFactory;
	UPROPERTY()
		class UUserWidget* aimingPointWidget;


	//UPROPERTY(VisibleAnywhere)
		//class UWidgetComponent* upgradeWidgetComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget>  upgradeWidgetComp;
	UPROPERTY()
		class UUpgradeWidget* upgradeWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget>  tutorialWidgetFactory;
	UPROPERTY()
		class UTutorialWidget* tutorialWidget;

	UPROPERTY(EditAnywhere, Category = "Setting")
		float Mode = 0;
	UPROPERTY()
		FTransform spaceSpawnPosition;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float fullHp = 200;
	UPROPERTY()
		float currentHp;
	UPROPERTY(EditAnywhere, Category = "Setting")
		float decreaseSpeed = 0.05;

	UPROPERTY()
		FVector2D MovementInput;
	UPROPERTY()
		FVector2D CameraInput;
	UPROPERTY()
		FVector2D aimingPointInput;
	UPROPERTY()
		float CameraInputRoll = 0;
	UPROPERTY()
		FVector dir = FVector(0, 0, 0);
	UPROPERTY()
		FVector dirHorizon = FVector(0, 0, 0);

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float Maxspeed = 300;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float Maxspeed_SpeedMode = 20000;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float Maxspeed_NormalMode = 300;
	UPROPERTY()
		float accel = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float normalModeAccel = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float speedModeAccel = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float decel = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float backSpeed = -100;
	UPROPERTY()
		float currentSpeed = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float horizontalMaxspeed = 2000;
	UPROPERTY()
		float currentHorizontalSpeed = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TSubclassOf <class APlayerBullet> bulletFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		TSubclassOf <class ABullet_Missile> MissileFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float maxMagazine = 20;
	UPROPERTY()
		float currentMagazine;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float fireRate = 0.2;
	UPROPERTY()
		float currentFireRate = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float currentBulletDamage = 5;
	UPROPERTY()
		bool isReloading = false;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float reloadingDelay = 3;
	UPROPERTY()
		float currentReloadingDelay = 0;
	UPROPERTY()
		float currentTurnValue_Horizontal = 0;
	UPROPERTY()
		float currentTurnValue_Vertical = 0;
	UPROPERTY()
		float maxTuringValue = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float missileSpread = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float missileCoolTime = 10;
	UPROPERTY()
		float currentMissileCoolTime = missileCoolTime;
	UPROPERTY()
		bool isMissileCoolTime = false;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float duringDarkScene = 1;
	UPROPERTY()
		float currentDuringDarkScene = 0;
	UPROPERTY()
		bool isFadeInOut = false;
	UPROPERTY()
		bool FlyingMode = false;		//0 : 일반모드  1: 가속모드
	UPROPERTY()
		bool isAbleControl = true;
	UPROPERTY()
		bool isInteraction = false;
	UPROPERTY()
		bool isPicked = false;
	UPROPERTY()
		bool isCanGetOff = false;
	UPROPERTY()
		bool isNeutralNone = false;

	UPROPERTY()
		class ASpaceWarGameModeBase* gameMode;
	UPROPERTY()
		class APlayerController* MyController;
	


	UPROPERTY()
		TArray <class AItem*> Item;
	UPROPERTY()
		uint8 ItemInteractionCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget> InteractionWidgetFactory;
	UPROPERTY()
		class UUserWidget* InteractionWidget;

	///공격받을 시 경고 UI
	UPROPERTY()
		float currentUnderAttackVisualTime = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		float underAttackVisualTime = 0.1;
	UPROPERTY()
		float currentUnderAttackOpacity = 0;
	UPROPERTY()
		float underAttackOpacitySpeed = 0.02;
	UPROPERTY()
		bool isUnderAttackOpacityWork = false;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float messagePrintTime = 3;
	UPROPERTY()
		float currentMessagePrintTime = 0;

	UPROPERTY()
		TArray <uint8> tempInventory_ItemNumber;
	UPROPERTY()
		TArray <uint8> tempInventory_ItemAmount;
	UPROPERTY()
		int tempMoney;

	UPROPERTY()
		bool isPlayer_HumanBoard = true;

	UPROPERTY()
		FTransform parkTransform;
	
	UPROPERTY()
		TArray <uint8> upgradeLevel;	//0 : damage   1 : firerate   2 : magazine   3 : fullhp


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget>  questListWidgetFactory;
	UPROPERTY()
		class UQuestListWidget* questListWidget;

	UPROPERTY()
		TArray <FQuestStruct> questData;
	UPROPERTY()
		TArray <uint8> receiveQuestNumber;
	UPROPERTY()
		bool isCanInteractionNPC;

	UPROPERTY()
		bool isSpaceTutorialDone;


	///////////FUNCTION

	UFUNCTION()
		void PitchCamera(float value);
	UFUNCTION()
		void YawCamera(float value);

	UFUNCTION()
		void InputHorizontal(float value);
	UFUNCTION()
		void InputVertical(float value);


	UFUNCTION()
		void Turn();

	UFUNCTION()
		void MoveForward(float value);

	UFUNCTION()
		void Move();

	UFUNCTION()
		void Fire();
	UFUNCTION()
		void FireMissile();
	UFUNCTION()
		void ReloadPress();

	UFUNCTION()
		void Dead();

	UFUNCTION()
		void FullHp();

	UFUNCTION()
		void FirePressing(float value);
	UFUNCTION()
		void FireWork();
	UFUNCTION()
		void Reloading();

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollisionEnter_Overlab(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollision_End(AActor* OtherActor);
	UFUNCTION()
		void OnCollision_During();

	UFUNCTION()
		void TakeDamage(float damage);

	UFUNCTION()
		void InteractionWidgetRemoveAdd(bool RemoveAdd);

	UFUNCTION()
		void ExploreEffect();

	UFUNCTION()
		void SetPlayerHP(float InputHP);
	UFUNCTION()
		float GetPlayerHP();
	UFUNCTION()
		float GetPlayerFullHP();
	UFUNCTION()
		void SetPlayerFullHP(float inputValue);
	UFUNCTION()
		float GetCurrentMagazine();
	UFUNCTION()
		float GetMaxMagazine();
	UFUNCTION()
		void SetMaxMagazine(float inputValue);
	UFUNCTION()
		float GetFireRate();
	UFUNCTION()
		void SetFireRate(float inputValue);
	UFUNCTION()
		float GetDamage();
	UFUNCTION()
		void SetDamage(float inputValue);
	UFUNCTION()
		float GetMissileCoolTime();
	UFUNCTION()
		float GetCurrentMissileCoolTime();

	UFUNCTION()
		TArray <uint8> GetUpgradeLevel();
	UFUNCTION()
		void SetUpgradeLevel(TArray <uint8> inputValue);

	UFUNCTION()
		FTransform GetSpaceSpawnPositon();
	UFUNCTION()
		APlayerController* GetPlayerController();

	UFUNCTION()
		void GiveUpController();
	UFUNCTION()
		void SetController();

	UFUNCTION()
		void ChangeFlyingMode();

	UFUNCTION()
		void InteractionEvent();

	UFUNCTION()
		void GetItemPressed();
	UFUNCTION()
		void GetItemReleased();

	UFUNCTION()
		void UnderAttackOpacityWork();
	UFUNCTION()
		void SetUnderAttackOpacityWork();
	UFUNCTION()
		float GetPlayerCurrentReloadTime();
	UFUNCTION()
		float GetPlayerReloadTime();

	UFUNCTION()
		void SetUIVisible(bool OffOn);	//0 : 안보임	1 : 보임
	UFUNCTION()
		void InventoryVisible();

	UFUNCTION()
		TArray <uint8> GetInventory_ItemNumber();
	UFUNCTION()
		TArray <uint8> GetInventory_ItemAmount();
	UFUNCTION()
		int GetMoney();

	UFUNCTION()
		void LoadInventory(TArray <uint8> inputItemNumber, TArray <uint8> inputItemAmount, int inputmoney);

	UFUNCTION(BlueprintCallable, Category = Code)
		void SetMessage(FString inputString);
	UFUNCTION()
		void MessageWork();

	UFUNCTION()
		void UpgradeWindowVisible();
	UFUNCTION()
		void UpgradeInit();

	UFUNCTION()
		void UpgradeFireRate();
	UFUNCTION()
		void UpgradeMaxHp();
	UFUNCTION()
		void UpgradeCurrentHp();
	UFUNCTION()
		void UpgradeMaxMagazine();
	UFUNCTION()
		void UpgradeDamage();

	UFUNCTION()
		void SetPlayer_HumanBoard(bool isBoard);
	UFUNCTION()
		bool GetPlayer_HumanBoard();

	UFUNCTION()
		void GetOffSpaceShip();
	UFUNCTION()
		void RandomFireVoid();
	UFUNCTION()
		void RandomImHitSound();
	UFUNCTION()
		void RandomVictorySound();
	UFUNCTION()
		void PickUpItem();

	UFUNCTION()
		void VisibleQuestList();
	UFUNCTION()
		void QuestCanClear();
	UFUNCTION()
		void AddRemoveQuest(uint8 inputQuestNumber, float inputRemoveEnemyNum, float inputRemoveCount);
	UFUNCTION()
		void AddCollectQuest(uint8 inputQuestNumber, float inputCollectItemNum, float inputCollectItemCount);
	UFUNCTION()
		TArray <uint8> GetReceiveQuestNumber();
	UFUNCTION()
		void QuestSynchronized(TArray <FQuestStruct> inputQuestData, TArray <uint8> inputreceiveQuestNumber);
	UFUNCTION()
		void AddList(FQuestStruct inputquestData, uint8 inputQuestNumber);
	UFUNCTION()
		void RemoveList(int32 inputQuestNumber);
	UFUNCTION()
		void ListSort();
	UFUNCTION()
		void MissileCoolTimeWork();

	UFUNCTION()
		void TutorialBegin();
	UFUNCTION()
		void TutorialEnd();
	UFUNCTION()
		void SetIsSpaceTurorialDone(bool inputIsSpaceTutorialDone);
	UFUNCTION()
		bool GetIsSpaceTurorialDone();

	UFUNCTION()
		void GamePause();

	UFUNCTION()
		void EraseSaveData();
	//////TEST


	UPROPERTY()
		float TurnYawValue = 0;
	UPROPERTY()
		float TurnPitchValue = 0;

	UPROPERTY()
		float overTurnPitch = 0;
	UPROPERTY()
		float overTurnYaw = 0;

	UPROPERTY()
		float turnRollSpeed = 0.05;
	UPROPERTY()
		float currentRoll = 0;;

};
