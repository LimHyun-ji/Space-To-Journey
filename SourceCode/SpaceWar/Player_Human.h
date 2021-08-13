// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player_Human.generated.h"


//내 파일

USTRUCT(Atomic)
struct FQuestStruct
{
	GENERATED_USTRUCT_BODY();

public:

	UPROPERTY()
		FText questName = FText::FromString("");


	UPROPERTY()
		uint8 targetEnemyNum1 = 0;
	UPROPERTY()
		uint8 RemoveGoal1 = 0;
	UPROPERTY()
		uint8 targetEnemyNum2 = 0;
	UPROPERTY()
		uint8 RemoveGoal2 = 0;

	UPROPERTY()
		uint8 needItemNum1 = 0;
	UPROPERTY()
		uint8 needItemAmount1 = 0;
	UPROPERTY()
		uint8 needItemNum2 = 0;
	UPROPERTY()
		uint8 needItemAmount2 = 0;

	UPROPERTY()
		bool isClear = false;
	UPROPERTY()
		bool isCanClear = false;

};

UCLASS()
class SPACEWAR_API APlayer_Human : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_Human(const FObjectInitializer& ObjectInitializer);

	//입력 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		FVector DirX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		FVector DirY;

	FVector MovementInput;

protected:

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget>  InventoryWidgetComp;
	UPROPERTY()
		class UInventory_Widget* InventoryWidget;

	UPROPERTY()
		class ASpaceWarGameModeBase* gameMode;
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystemComponent* LandingEffect;

	//입력매핑 함수
	UFUNCTION()
		void CameraMove_Pitch(float Value);
	UFUNCTION()
		void CameraMove_Yaw(float Value);

	UFUNCTION()
		void Vertical(float Value);
	UFUNCTION()
		void Horizontal(float Value);

	UFUNCTION()
		void Fire();

	UFUNCTION()
		void ChangeRunningMode();
	//상호작용
	UFUNCTION()
		void GetItemPressed();
	UFUNCTION()
		void GetItemReleased();
	UFUNCTION()
		void ReturnReleased();
	UFUNCTION()
		void ReturnPressed();
	UFUNCTION()
		void Attack();
	UFUNCTION()
		void SkillAttack1();
	UFUNCTION()
		void SkillAttack2();

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnCollision(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnOverlap_Begin(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Code")
		void  OnOverlap_End(AActor* OtherActor);

	UFUNCTION()
		void InteractionWidgetRemoveAdd(bool RemoveAdd);	//remove : 0	add : 1
	UFUNCTION()
		void HitWidgetRemoveAdd(bool RemoveAdd);	//remove : 0	add : 1
	UFUNCTION()
		void PlayerWidgetRemoveAdd(bool RemoveAdd);

	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsDead();

	UFUNCTION()
		void InventoryVisible();
	UFUNCTION()
		float GetPlayerFullHP();
	UFUNCTION()
		float GetPlayerHP();
	UFUNCTION(BluePrintCallable, Category = "Code")
		float GetPlayerMP();
	UFUNCTION()
		float GetPlayerFullMP();
	UFUNCTION()
		void SetPlayerMP(int inputMP);

	UFUNCTION()
		void LoadInventory(TArray <uint8> inputItemNumber, TArray <uint8> inputItemAmount,int inputMoney);

	UFUNCTION()
		TArray <uint8> GetInventory_ItemNumber();
	UFUNCTION()
		TArray <uint8> GetInventory_ItemAmount();
	UFUNCTION()
		int GetMoney();

	UFUNCTION()
		APlayerController* GetPlayerController();
	UFUNCTION()
		void BoardAShip();
	UFUNCTION()
		void GetOnVehicle(class AVehicle* inputVehicle);
	UFUNCTION()
		void TalkNPC();
	UFUNCTION()
		void TalkNPC_End();
	UFUNCTION()
		void SetController();
	UFUNCTION()
		void SetUIVisible(bool OnOff);
	UFUNCTION()
		void PickUpItem();
	UFUNCTION()
		void SetIsGetOnVehicle(bool inputOnOff);
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsGetOnVehicle();
	UFUNCTION()
		void SetBulletCount(int count);
	UFUNCTION()
		int GetBulletCount();
	UFUNCTION()
		int GetMaxBulletCount();
	UFUNCTION(BlueprintCallable, Category = "Code")
		bool GetIsHit();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void TakeDamage(float inputDamage);

	UFUNCTION()
		void PressAttack1Activate();
	UFUNCTION()
		void PressAttack2Activate();
	UFUNCTION()
		void Reload();
	UFUNCTION()
		void ReloadPress();

	UFUNCTION(BlueprintCallable, Category = "Code")
		void RemoveInventory(uint8 removeItemNumber, uint8 removeCount);
	UFUNCTION(BlueprintCallable, Category = "Code")
		float GetItemAmount(uint8 inputItemNumber);

	UFUNCTION()
		void QuestCanClear();
	UFUNCTION()
		void QuestClear();
	UFUNCTION()
		void QuestButtonVisible();
	UFUNCTION()
		void AddRemoveQuest(uint8 inputQuestNumber, float inputRemoveEnemyNum, float inputRemoveCount, FText inputQuestName);
	UFUNCTION()
		void AddCollectQuest(uint8 inputQuestNumber, float inputCollectItemNum, float inputCollectItemCount, FText inputQuestName);
	UFUNCTION()
		TArray <uint8> GetReceiveQuestNumber();
	UFUNCTION()
		void SetVisibleQuestWindow(bool inputOnOff);
	UFUNCTION()
		void SettingQuestText(FText inputNPCName, FText inputTitle = FText::FromString(""), FText inputConversation = FText::FromString(""), FText inputGoalText = FText::FromString(""), FText inputGoalCount = FText::FromString(""));
	UFUNCTION()
		void AcceptQuest();
	UFUNCTION()
		void QuestSynchronized(TArray <FQuestStruct> inputQuestData, TArray <uint8> inputreceiveQuestNumber);
	UFUNCTION()
		bool IsClearQuest(uint8 inputQuestNumber);
	UFUNCTION()
		void VisibleQuestList();

	UFUNCTION()
		void InteractDragonKeyPressed();
	UFUNCTION()
		void InteractDragonKeyReleased();
	UFUNCTION()
		bool GetIscatchDragon();

	UFUNCTION()
		void TutorialBegin();
	UFUNCTION()
		void TutorialEnd();
	UFUNCTION()
		void SetIsSpaceTurorialDone(bool inputIsHumanTutorialDone);
	UFUNCTION()
		bool GetIsSpaceTurorialDone();

	UFUNCTION()
		void GamePause();

	

	UFUNCTION()
		void EraseSaveData();

	//컴포넌트 객체
	//SpringArm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* SpringArm;

	//Follow Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* FirePosition;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UArrowComponent* SkillAttack1Position;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AH_PlayerBullet> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AH_PlayerBullet> SkillProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = SkillAttack)
		TSubclassOf<class ASkillAttack1> SkillAttack1Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget> InteractionWidgetFactory;
	UPROPERTY()
		class UUserWidget* InteractionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget> HitWidgetFactory;
	UPROPERTY()
		class UUserWidget* HitWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget>  questWidgetFactory;
	UPROPERTY()
		class UQuestWidget* questWidget;

	

	UPROPERTY()
		TArray <class AItem*> Item;
	UPROPERTY()
		uint8 ItemInteractionCount = 0;
	UPROPERTY()
		class APlayerCPP* SpaceShip;
	UPROPERTY(EditDefaultsOnly)
		class APlanet_EnemyParent* EnemyAnimal;
	UPROPERTY(EditDefaultsOnly)
		class AEnemy_FarAwayAttack* EnemyFarAwayAttack;
	UPROPERTY(EditDefaultsOnly)
		class AEnemy_ThrowingBall* EnemyThrowingAttack;
	UPROPERTY()
		class ABulletBox* BulletBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget>  Player_HumanUIWidgetFactory;
	UPROPERTY()
		class UPlayer_HumanUI* Player_HumanUI;
	UPROPERTY()
		class AVehicle* vehicle;

	//걷기, 뛰기 모드 두 가지
	UPROPERTY()
		bool RunningMode = false;

	//카메라 위치에서의 총구 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;
	UPROPERTY()
		FVector CameraLocation;
	UPROPERTY()
		FRotator CameraRotation;

	//Returns SprimgArm subobject
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	//Returns FollowCamera subobject
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY()
		bool isInteraction = false;
	UPROPERTY()
		bool isPicked;
	UPROPERTY()
		bool isReturned;
	UPROPERTY(EditDefaultsOnly)
		int playerFullHP = 50;
	UPROPERTY()
		int playerHP;	
	UPROPERTY(EditDefaultsOnly)
		float playerFullMP = 100;
	UPROPERTY()
		float playerMP;
	UPROPERTY(EditDefaultsOnly)
		float countDown = 1.5;
	UPROPERTY(EditDefaultsOnly)
		bool isDead;
	UPROPERTY(EditDefaultsOnly)
		bool isHit;

	UPROPERTY()
		float attack1CountDown = 0.5f;
	UPROPERTY()
		float attack2CountDown = 0.7f;
	UPROPERTY()
		bool  isAttack1Activate = false;
	UPROPERTY()
		bool isAttack2Activate = false;
	UPROPERTY(EditDefaultsOnly)
		float isHitCountDown = 2.0f;
	UPROPERTY()
		int bulletCount;
	UPROPERTY()
		int maxBulletCount = 20;
	UPROPERTY()
		bool isReloading = false;
	UPROPERTY()
		float reloadDelay = 2.0f;
	UPROPERTY()
		float currentReloadDelay = 0.0f;

	UPROPERTY(EditDefaultsOnly)
		bool isGetOnVehicle;
	UPROPERTY()
		bool isCanGetOnVehicle;
	UPROPERTY()
		bool isCanBoardSpaceShip;

	UPROPERTY(EditDefaultsOnly)
		uint8 currentOverlapCount = 0;
	UPROPERTY()
		float HitWidgetTime = 0.0f;

	UPROPERTY()
		TArray <uint8> tempInventory_ItemNumber;
	UPROPERTY()
		TArray <uint8> tempInventory_ItemAmount;
	UPROPERTY()
		int tempMoney;
	UPROPERTY()
		class APlayerController* MyController;

	/// quest
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
		class ANPC_Parent* NPC;
	UPROPERTY()
		bool isCanControll = true;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* AimingPointWidgetFactory;
	UPROPERTY()
		class UUserWidget* AimingPointWidget;


	UPROPERTY()
		int interactNum;
	UPROPERTY()
		FString order = "";
	UPROPERTY(EditDefaultsOnly, Category = "Order")
		FString correctOrder = "1234";
	UPROPERTY()
		class ADragonInteractionArea* dragonKey;
	UPROPERTY()
		bool isInteractDragonKey = false;
	UPROPERTY(EditdefaultsOnly, Category = "Order")
		uint8 dragonKeyTotalCount = 4;
	UPROPERTY()
		bool isCatchDragon = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting")
		TSubclassOf<class UUserWidget>  tutorialWidgetFactory;
	UPROPERTY()
		class UTutorialWidget* tutorialWidget;
	UPROPERTY()
		bool isPlanetTutorialDone;
};
