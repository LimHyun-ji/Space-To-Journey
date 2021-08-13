// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Human.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/ArrowComponent.h>
#include "Blueprint/UserWidget.h"
#include <Components/WidgetComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include <GameFramework/Character.h>
#include "SpaceWarGameModeBase.h"
#include "PlayerBullet.h"
#include "Item.h"
#include "PlayerCPP.h"
#include "Inventory_Widget.h"
#include "Player_HumanUI.h"
#include "Vehicle.h"
#include "Enemy_Animal.h"
#include "Enemy_FarAwayAttack.h"
#include "Enemy_ThrowingBall.h"
#include "H_PlayerBullet.h"
#include "BulletBox.h"
#include "QuestWidget.h"
#include "NPC_Parent.h"
#include "QuestListWidget.h"
#include <Particles/ParticleSystemComponent.h>
#include "SkillAttack1.h"
#include "DragonFire.h"
#include "Enemy_BulletBall.h"
#include "Enemy_Dragon.h"
#include "DragonInteractionArea.h"
#include "TutorialWidget.h"
#include "Planet_EnemyParent.h"
//내 파일

// Sets default values
APlayer_Human::APlayer_Human(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;


	UCapsuleComponent* CapComp = GetCapsuleComponent();
	CapComp->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//configureActor Movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.f, 0.f));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;//rotate arm based on the controller
	//SpringArm->bEnableCameraLag = true;
	//SpringArm->CameraLagSpeed = 3.0f;

	//Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;// Camera does not rotate relative to arm

	FirePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePosition"));
	FirePosition->SetupAttachment(RootComponent);

	SkillAttack1Position = CreateDefaultSubobject<UArrowComponent>(TEXT("Skill1Position"));
	SkillAttack1Position->SetupAttachment(RootComponent);
	
	AimingPointWidgetFactory = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("AimingPoint"));
	AimingPointWidgetFactory->AttachToComponent(FirePosition, FAttachmentTransformRules::KeepRelativeTransform);

	LandingEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LandingEffect"));
	LandingEffect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayer_Human::BeginPlay()
{
	Super::BeginPlay();

	gameMode = Cast<ASpaceWarGameModeBase>(GetWorld()->GetAuthGameMode());
	MyController = Cast<APlayerController>(Controller);
	

	TArray <AActor*> obj;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCPP::StaticClass(), obj);
	for (AActor* actor : obj)
	{
		SpaceShip = Cast<APlayerCPP>(actor);

		if (IsValid(SpaceShip))
			break;
	}

	if (Player_HumanUIWidgetFactory)
	{
		Player_HumanUI = CreateWidget<UPlayer_HumanUI>(GetWorld(), Player_HumanUIWidgetFactory);
		Player_HumanUI->SetOwner(this);
		Player_HumanUI->AddToViewport();
	}	

	if (InteractionWidgetFactory)
	{
		InteractionWidget = CreateWidget<UUserWidget>(GetWorld(), InteractionWidgetFactory);
	}

	
	if (questWidgetFactory)
	{
		questWidget = CreateWidget<UQuestWidget>(GetWorld(), questWidgetFactory);
		questWidget->SetWidgetOwner(this);
	}

	if (questListWidgetFactory)
	{
		questListWidget = CreateWidget<UQuestListWidget>(GetWorld(), questListWidgetFactory);
		//questListWidget->SetWidgetOwner(this);
	}

	if (HitWidgetFactory)
	{
		HitWidget = CreateWidget<UUserWidget>(GetWorld(), HitWidgetFactory);
	}
	
	isReturned = false;
	isPicked = false;

	if (InventoryWidgetComp)
	{
		InventoryWidget = CreateWidget<UInventory_Widget>(GetWorld(), InventoryWidgetComp);
		InventoryWidget->AddToViewport();
		InventoryWidget->SetInventoryVisible(0);
	}

	if (tutorialWidgetFactory)
	{
		tutorialWidget = CreateWidget<UTutorialWidget>(GetWorld(), tutorialWidgetFactory);
		tutorialWidget->SetOwner_Human(this);
	}

	AimingPointWidget = Cast<UUserWidget>(AimingPointWidgetFactory->GetUserWidgetObject());

	gameMode->LoadData_Human();
	
	InventoryWidget->LoadInventory(tempInventory_ItemNumber, tempInventory_ItemAmount, tempMoney);

	//if (!IsValid(MyController))	Player_HumanUI->SetVisibility(ESlateVisibility::Hidden);
	if (!IsValid(MyController))	SetUIVisible(0);

	isReturned = false;
	isPicked = false;
	SetBulletCount(20);
	
	playerHP = playerFullHP;
	bulletCount = maxBulletCount;
	playerHP = playerFullHP;
	playerMP = 100;

	if (GetWorld()->GetMapName().Contains(TEXT("Planet")))
	{
		if (!isPlanetTutorialDone) TutorialBegin();
	}
}

// Called every frame
void APlayer_Human::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//OnCollision();

	playerMP += 0.1;

	if (isInteraction)
	{
		if (isPicked && ItemInteractionCount != 0)
		{
			PickUpItem();
		}

		if (IsValid(SpaceShip) && isReturned)
		{
			InteractionWidgetRemoveAdd(0);
			if (GetWorld()->GetMapName().Contains(TEXT("Space")))
			{
				BoardAShip();
			}
			else
			{
				gameMode->LoadNewLevel("TheSpace");
			}
			
		}

		if (isInteractDragonKey)
		{			
			if (dragonKey)
			{
				if (dragonKey->canInteract)
					order += FString::FromInt(dragonKey->orderNum);
				dragonKey->canInteract = false;
				if (order.Len() == dragonKeyTotalCount)
				{
					if (order == correctOrder)
					{
						//용깨우기(last dragonKey)
						dragonKey->wakeUpDragon = true;
					}
					else
						order = "";
				}
			}

		}
	}

	if (isReloading)
		Reload();

	if (playerMP > playerFullMP)
	{
		playerMP = playerFullMP;
	}

	if (playerHP < 0)
	{
		isDead = true;
		countDown -= GetWorld()->DeltaTimeSeconds;

		if (countDown < 0)
			Destroy();
	}

	if (playerHP < 0)
	{
		isDead = true;
		HitWidgetRemoveAdd(0);

		countDown -= GetWorld()->DeltaTimeSeconds;

		if (countDown < 0)
			Destroy();
	}

	if (isAttack1Activate)
	{
		attack1CountDown -= GetWorld()->DeltaTimeSeconds;
		if (attack1CountDown < 0)
		{
			SkillAttack1();
			attack1CountDown = 0.5f;
			isAttack1Activate = false;
		}
	}
	if (isAttack2Activate)
	{
		attack2CountDown -= GetWorld()->DeltaTimeSeconds;
		if (attack2CountDown < 0)
		{
			SkillAttack2();
			attack2CountDown = 0.7f;
			isAttack2Activate = false;
		}
	}
	/////////
	if (!isHit)
	{
		//HitEffectTime
		HitWidgetTime += GetWorld()->DeltaTimeSeconds;
		if (HitWidgetTime > 1.0f)
		{
			HitWidgetRemoveAdd(0);
			HitWidgetTime = 0;
		}
	}

}

// Called to bind functionality to input
void APlayer_Human::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("Vertical", this, &APlayer_Human::Vertical);
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayer_Human::Horizontal);
	PlayerInputComponent->BindAction("FlyingMode", IE_Pressed, this, &APlayer_Human::ChangeRunningMode);

	PlayerInputComponent->BindAxis("Turn", this, &APlayer_Human::CameraMove_Yaw);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayer_Human::CameraMove_Pitch);

	PlayerInputComponent->BindAction("Fire_H", IE_Pressed, this, &APlayer_Human::Fire);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &APlayer_Human::ReturnPressed);
	PlayerInputComponent->BindAction("Interaction", IE_Released, this, &APlayer_Human::ReturnReleased);

	PlayerInputComponent->BindAction("GetItem", IE_Pressed, this, &APlayer_Human::GetItemPressed);
	PlayerInputComponent->BindAction("GetItem", IE_Released, this, &APlayer_Human::GetItemReleased);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &APlayer_Human::InventoryVisible);
	PlayerInputComponent->BindAction("QuestList", IE_Pressed, this, &APlayer_Human::VisibleQuestList);

	PlayerInputComponent->BindAction("SkillAttack1", IE_Pressed, this, &APlayer_Human::PressAttack1Activate);
	PlayerInputComponent->BindAction("SkillAttack2", IE_Pressed, this, &APlayer_Human::PressAttack2Activate);

	PlayerInputComponent->BindAction("InteractDragonKey", IE_Pressed, this, &APlayer_Human::InteractDragonKeyPressed);
	PlayerInputComponent->BindAction("InteractDragonKey", IE_Released, this, &APlayer_Human::InteractDragonKeyReleased);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayer_Human::GamePause);


	PlayerInputComponent->BindAction("SaveDataErase", IE_Pressed, this, &APlayer_Human::EraseSaveData);
}

void APlayer_Human::CameraMove_Pitch(float Value)
{	
	if (!isCanControll)	return;
	AddControllerPitchInput(Value);

	FRotator newFirePositionRotation = FirePosition->GetRelativeRotation();
	newFirePositionRotation.Pitch = FollowCamera->GetComponentRotation().Pitch+10;
	FirePosition->SetRelativeRotation(newFirePositionRotation);
}

void APlayer_Human::CameraMove_Yaw(float Value)
{
	if (!isCanControll)	return;
	AddControllerYawInput(Value);
}

void APlayer_Human::Vertical(float Value)
{	
	if (!isCanControll)	return;

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		DirX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(DirX, Value);

	}

}

void APlayer_Human::Horizontal(float Value)
{
	if (!isCanControll)	return;

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		DirY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(DirY, Value);
	}
}

void APlayer_Human::ReturnPressed()
{
	if (isCanBoardSpaceShip)
	{
		isReturned = true;
	}
	else if (isCanGetOnVehicle)
	{
		GetOnVehicle(vehicle);
	}
	else if (isCanInteractionNPC)
	{			
		MyController->SetShowMouseCursor(true);		
		if(questWidget->IsInViewport()) TalkNPC_End();
		else TalkNPC();

		NPC->UpdateTalk(this);
		
	}
}

void APlayer_Human::Attack()
{/*
		UAnimInstanceCPP* myAnimInst = Cast<UAnimInstanceCPP>(animInstance);
	if (myAnimInst != nullptr)
	{
		myAnimInst->Attack();
	}*/
}

void APlayer_Human::SkillAttack1()
{
	//광범위공격
	//mp가 100일 때만 실행
	if (playerMP >= 100)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		//총구 위치에 발사체 spawn
		ASkillAttack1* Attack = GetWorld()->SpawnActor<ASkillAttack1>(SkillAttack1Class, SkillAttack1Position->GetComponentTransform(), SpawnParams);
		playerMP -= 100;
	}

}

void APlayer_Human::SkillAttack2()
{
	//집중타겟
	if (playerMP >= 50)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		//총구 위치에 발사체 spawn

		AH_PlayerBullet* Attack = GetWorld()->SpawnActor<AH_PlayerBullet>(SkillProjectileClass, FirePosition->GetComponentTransform(), SpawnParams);
		playerMP -= 50;
	}
}

void APlayer_Human::ReturnReleased()
{
	isReturned = false;
}

void APlayer_Human::Fire()
{
	if (!isCanControll)	return;

	if (ProjectileClass)//클래스가 있다면, 총알이 남아있다면
	{
		if (bulletCount > 0)
		{
			GetActorEyesViewPoint(CameraLocation, CameraRotation);

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			//총구 위치에 발사체 spawn

			
			AH_PlayerBullet* Projectile = GetWorld()->SpawnActor<AH_PlayerBullet>(ProjectileClass, FirePosition->GetComponentTransform(), SpawnParams);

			bulletCount--;
			if (bulletCount <= 0)
				isReloading = true;

			/*
			AH_PlayerBullet* Projectile = GetWorld()->SpawnActor<AH_PlayerBullet>(ProjectileClass, FirePosition->GetComponentTransform(), SpawnParams);

			//총발사 위아래를 카메라에 따라 동작하기
			if (CameraRotation.Pitch < 0)
				CameraRotation.Pitch -= 20;
			else
				CameraRotation.Pitch += 20;
			const FRotator PitchRotation(CameraRotation.Pitch- 18, GetActorRotation().Yaw, 0);
			Projectile->SetActorRotation(PitchRotation);
			bulletCount--;
			if (bulletCount <= 0)
				isReloading = true;
				*/

		}

	}
}

void APlayer_Human::ChangeRunningMode()
{
	if (!isCanControll)	return;

	RunningMode = !RunningMode;
}



void APlayer_Human::OnOverlap_Begin(AActor* OtherActor)
{
	if (OtherActor != this)
	{
		AItem* itemTemp = Cast<AItem>(OtherActor);
		APlayerCPP* SpaceShipTemp = Cast<APlayerCPP>(OtherActor);
		AVehicle* vehicleTemp = Cast<AVehicle>(OtherActor);
		ANPC_Parent* NPCTemp = Cast<ANPC_Parent>(OtherActor);
		EnemyFarAwayAttack = Cast<AEnemy_FarAwayAttack>(OtherActor);
		EnemyThrowingAttack = Cast<AEnemy_ThrowingBall>(OtherActor);
		EnemyAnimal = Cast<APlanet_EnemyParent>(OtherActor);
		ADragonFire* dragonFire = Cast<ADragonFire>(OtherActor);
		AEnemy_BulletBall* dragonClaw = Cast<AEnemy_BulletBall>(OtherActor);
		AEnemy_Dragon* dragon = Cast<AEnemy_Dragon>(OtherActor);
		dragonKey = Cast<ADragonInteractionArea>(OtherActor);

		if (IsValid(itemTemp))
		{
			isInteraction = true;

			Item.Add(itemTemp);
			InteractionWidgetRemoveAdd(1);
			ItemInteractionCount++;			

		}
		else if (IsValid(SpaceShipTemp))
		{
			isInteraction = true;
			isCanBoardSpaceShip = true;
			InteractionWidgetRemoveAdd(1);
		}
		else if (IsValid(vehicleTemp) && !isGetOnVehicle)
		{
			isInteraction = true;
			isCanGetOnVehicle = true;
			InteractionWidgetRemoveAdd(1);
			vehicle = vehicleTemp;
		}
		else if (IsValid(NPCTemp))
		{
			isInteraction = true;
			InteractionWidgetRemoveAdd(1);
			isCanInteractionNPC = true;
			NPC = NPCTemp;
			
			QuestButtonVisible();

		}
		else if (IsValid(dragonKey))
		{
			isInteraction = true;
			InteractionWidgetRemoveAdd(1);
		}
		else if (OtherActor == EnemyFarAwayAttack)
		{
			isHit = true;
			playerHP -= EnemyFarAwayAttack->GetAttackDamage();
			HitWidgetRemoveAdd(1);
			//공격을 피하는 방법은 없나?
		}
		else if (OtherActor == EnemyThrowingAttack)
		{
			isHit = true;
			playerHP -= EnemyThrowingAttack->GetAttackDamage();
			HitWidgetRemoveAdd(1);
		}
		else if (OtherActor == EnemyAnimal)
		{
			//Animal에서 HP 깎음
			isHit = true;
			HitWidgetRemoveAdd(1);
		}
		else if (OtherActor == dragonFire)
		{
			isHit = true;
			playerHP -= dragonFire->GetAttackDamage();
			HitWidgetRemoveAdd(1);
		}
		else if (OtherActor == dragonClaw)
		{
			isHit = true;
			playerHP -= dragonClaw->GetBulletDamage();
			HitWidgetRemoveAdd(1);
		}
		else if (OtherActor == dragon && dragon->isInteracting)
		{
			//dragonBiteSpawn에서HP깎음
			isHit = true;
			HitWidgetRemoveAdd(1);
		}
	}
}

void APlayer_Human::OnOverlap_End(AActor* OtherActor)
{
	if (OtherActor != this)
	{
		AItem* tempItem = Cast<AItem>(OtherActor);
		APlayerCPP* SpaceShipTemp = Cast<APlayerCPP>(OtherActor);
		AVehicle* vehicleTemp = Cast<AVehicle>(OtherActor);
		ANPC_Parent* NPCTemp = Cast<ANPC_Parent>(OtherActor);
		EnemyFarAwayAttack = Cast<AEnemy_FarAwayAttack>(OtherActor);
		EnemyThrowingAttack = Cast<AEnemy_ThrowingBall>(OtherActor);
		EnemyAnimal = Cast<APlanet_EnemyParent>(OtherActor);
		ADragonFire* dragonFire = Cast<ADragonFire>(OtherActor);
		AEnemy_BulletBall* dragonClaw = Cast<AEnemy_BulletBall>(OtherActor);
		AEnemy_Dragon* dragon = Cast<AEnemy_Dragon>(OtherActor);
		dragonKey = Cast<ADragonInteractionArea>(OtherActor);

		if (IsValid(tempItem))
		{
			Item.Remove(tempItem);
			ItemInteractionCount--;
			if (ItemInteractionCount == 0)
			{
				InteractionWidgetRemoveAdd(0);
				isInteraction = false;
			}
		}
		else if (IsValid(SpaceShipTemp))
		{
			isCanBoardSpaceShip = false;
			isInteraction = false;
			InteractionWidgetRemoveAdd(0);
		}
		else if (IsValid(vehicleTemp))
		{
			isInteraction = false;
			isCanGetOnVehicle = false;
			InteractionWidgetRemoveAdd(0);
		}
		else if (IsValid(NPCTemp))
		{
			isInteraction = false;
			InteractionWidgetRemoveAdd(0);
			isCanInteractionNPC = false;
			TalkNPC_End();
		}
		else if (IsValid(dragonKey))
		{
			isInteraction = false;
			InteractionWidgetRemoveAdd(0);
		}
		else if (OtherActor == EnemyFarAwayAttack || OtherActor == EnemyThrowingAttack || OtherActor == EnemyAnimal || dragonFire || dragonClaw || dragon)
		{
			isHit = false;
		}
	}
}


void APlayer_Human::OnCollision(AActor* OtherActor)
{
	EnemyAnimal = Cast<APlanet_EnemyParent>(OtherActor);

	if (OtherActor != this)
	{
		if (OtherActor == EnemyAnimal)
		{
			isHit = true;//제대로 동작하기는 하는데 isHit false는 어디서 해줄 것인가?>>tick에서 하면 overlapevent 가 안됨
		}
	}
}

void APlayer_Human::Reload()
{
	currentReloadDelay += GetWorld()->DeltaTimeSeconds;
	if (currentReloadDelay > reloadDelay)
	{
		SetBulletCount(maxBulletCount);
		currentReloadDelay = 0.0f;
		isReloading = false;
	}
}

void APlayer_Human::ReloadPress()
{
	isReloading = true;
}

/////////////////////

void APlayer_Human::GetItemPressed()
{
	isPicked = true;		
}

void APlayer_Human::GetItemReleased()
{
	isPicked = false;
}

void APlayer_Human::InteractionWidgetRemoveAdd(bool RemoveAdd)
{

	if (RemoveAdd && !InteractionWidget->IsInViewport())
	{
		InteractionWidget->AddToViewport();
	}
	else if (!RemoveAdd && InteractionWidget->IsInViewport())
	{
		InteractionWidget->RemoveFromViewport();
	}

}

void APlayer_Human::HitWidgetRemoveAdd(bool RemoveAdd)
{
	if (RemoveAdd && !HitWidget->IsInViewport())
	{
		HitWidget->AddToViewport();
	}
	else if (!RemoveAdd && HitWidget->IsInViewport())
	{
		HitWidget->RemoveFromViewport();
	}

}
void APlayer_Human::PlayerWidgetRemoveAdd(bool RemoveAdd)
{
	if (RemoveAdd && !Player_HumanUI->IsInViewport())
	{
		Player_HumanUI->AddToViewport();
	}
	else if (!RemoveAdd && Player_HumanUI->IsInViewport())
	{
		Player_HumanUI->RemoveFromViewport();
	}

}

bool APlayer_Human::GetIsDead()
{
	return isDead;
}

void APlayer_Human::InventoryVisible()
{	
	if (!isCanControll)	return;

	if (InventoryWidget->IsVisible())
	{
		InventoryWidget->SetInventoryVisible(0);
		MyController->SetShowMouseCursor(false);
	}		
	else
	{
		InventoryWidget->SetInventoryVisible(1);
		MyController->SetShowMouseCursor(true);
	}		
}


float APlayer_Human::GetPlayerMP()
{
	return playerMP;
}

float APlayer_Human::GetPlayerFullMP()
{
	return playerFullMP;
}

void APlayer_Human::SetPlayerMP(int inputMP)
{
	playerMP = inputMP;
}

float APlayer_Human::GetPlayerHP()
{
	return playerHP;
}

float APlayer_Human::GetPlayerFullHP()
{
	return playerFullHP;
}

void APlayer_Human::SetBulletCount(int count)
{
	this->bulletCount = count;
}

int APlayer_Human::GetMaxBulletCount()
{
	return maxBulletCount;
}

int APlayer_Human::GetBulletCount()
{
	return bulletCount;
}

bool APlayer_Human::GetIsHit()
{
	return isHit;
}

void APlayer_Human::PressAttack1Activate()
{
	isAttack1Activate = true;
}

void APlayer_Human::PressAttack2Activate()
{
	isAttack2Activate = true;
}

void APlayer_Human::LoadInventory(TArray <uint8> inputItemNumber, TArray <uint8> inputItemAmount, int inputMoney)
{
	tempInventory_ItemNumber = inputItemNumber;
	tempInventory_ItemAmount = inputItemAmount;
	tempMoney = inputMoney;
}

void APlayer_Human::PickUpItem()
{
	if (IsValid(InventoryWidget))
	{
		InventoryWidget->AddInventory(Item[0]->GetItemNumber());
	}
	isPicked = false;
	Item[0]->Destroy();
	
}

TArray <uint8> APlayer_Human::GetInventory_ItemNumber()
{
	return InventoryWidget->GetInventory_ItemNumber();
}

TArray <uint8> APlayer_Human::GetInventory_ItemAmount()
{
	return InventoryWidget->GetInventory_ItemAmount();
}

int APlayer_Human::GetMoney()
{
	return InventoryWidget->GetMoney();
}

APlayerController* APlayer_Human::GetPlayerController()
{
	return MyController;
}

void APlayer_Human::BoardAShip()
{		
	if (!SpaceShip->GetPlayer_HumanBoard())
	{
		SetUIVisible(0);

		MyController->UnPossess();
		MyController->Possess(SpaceShip);
		SpaceShip->SetController();
		SpaceShip->SetUIVisible(1);
		SpaceShip->SetPlayer_HumanBoard(true);
	}	
}
void APlayer_Human::GetOnVehicle(AVehicle* inputVehicle)
{
	vehicle = inputVehicle;

	SetUIVisible(0);

	MyController->UnPossess();
	MyController->Possess(vehicle);
	vehicle->SetController(MyController);
	vehicle->SetActorRotation(GetActorRotation());
	SetIsGetOnVehicle(true);
	vehicle->SetGetOnHuman(this);
	
}

void APlayer_Human::TalkNPC()
{

	isCanControll = false;
	SetUIVisible(0);
	SetVisibleQuestWindow(1);
	MyController->SetViewTarget(NPC);	
}

void APlayer_Human::TalkNPC_End()
{

	isCanControll = true;
	SetUIVisible(1);
	SetVisibleQuestWindow(0);
	MyController->SetViewTarget(this);
	MyController->SetShowMouseCursor(false);
}

void APlayer_Human::SettingQuestText(FText inputNPCName, FText inputTitle, FText inputConversation, FText inputGoalText, FText inputGoalCount)
{
	questWidget->SettingQuestText(inputNPCName, inputTitle, inputConversation, inputGoalText, inputGoalCount);
}

void APlayer_Human::AcceptQuest()
{
	NPC->AddQuest(this);
	SpaceShip->QuestSynchronized(questData, receiveQuestNumber);

	uint8 count = 0;
	for (FQuestStruct quest : questData)
	{
		count++;
	}

	QuestButtonVisible();
	NPC->UpdateTalk(this);
}

bool APlayer_Human::IsClearQuest(uint8 inputQuestNumber)
{
	int32 index;
	if (receiveQuestNumber.Find(inputQuestNumber, index))
		if (questData[index].isClear)	return true;

	return false;
}

void APlayer_Human::QuestButtonVisible()
{
	int32 index;

	if (!NPC->IsGiveQuestNPC())
	{
		questWidget->VisibleAcceptBtn(0);
		questWidget->VisibleClearBtn(0);
	}
	else if (receiveQuestNumber.Find(NPC->GetQuestNumber(), index))
	{
		questWidget->VisibleAcceptBtn(0);

		if (questData[index].isClear) questWidget->VisibleClearBtn(0);
		else if (questData[index].isCanClear) questWidget->VisibleClearBtn(1);
	}
	else
	{
		questWidget->VisibleAcceptBtn(1);
		questWidget->VisibleClearBtn(0);
	}
}

void APlayer_Human::QuestSynchronized(TArray <FQuestStruct> inputQuestData, TArray <uint8> inputreceiveQuestNumber)
{
	questData = inputQuestData;
	receiveQuestNumber = inputreceiveQuestNumber;
}

void APlayer_Human::VisibleQuestList()
{
	if (questListWidget->IsInViewport())
	{
		questListWidget->SetVisibleQuestList(0);
	}
	else
	{
		questListWidget->SetVisibleQuestList(1);
	}
}

void APlayer_Human::SetController()
{
	MyController = GetWorld()->GetFirstPlayerController();
}

void APlayer_Human::SetUIVisible(bool OnOff)
{
	if (!OnOff)
	{		
		InventoryWidget->SetInventoryVisible(0);
		Player_HumanUI->SetVisibility(ESlateVisibility::Hidden);
		AimingPointWidget->SetVisibility(ESlateVisibility::Hidden);
		if (InteractionWidget->IsInViewport())	InteractionWidget->RemoveFromViewport();
		questListWidget->SetVisibleQuestList(0);		
	}
	else
	{		
		Player_HumanUI->SetVisibility(ESlateVisibility::Visible);
		AimingPointWidget->SetVisibility(ESlateVisibility::Visible);
		if(isInteraction) InteractionWidget->AddToViewport();		
	}
	
}

void APlayer_Human::SetIsGetOnVehicle(bool inputOnOff)
{
	isGetOnVehicle = inputOnOff;
}


bool APlayer_Human::GetIsGetOnVehicle()
{
	return isGetOnVehicle;
}

void APlayer_Human::TakeDamage(float inputDamage)
{
	playerHP -= inputDamage;
}

void APlayer_Human::RemoveInventory(uint8 removeItemNumber, uint8 removeCount)
{
	InventoryWidget->RemoveInventory(removeItemNumber, removeCount);
}

float APlayer_Human::GetItemAmount(uint8 inputItemNumber)
{
	return InventoryWidget->GetItemAmount(inputItemNumber);
}

///quest

void APlayer_Human::QuestCanClear()
{
	//나중에 아이템 수집 퀘스트에 사용할듯
}

void APlayer_Human::QuestClear()
{
	int32 index;
	if (receiveQuestNumber.Find(NPC->GetQuestNumber(), index))
	{
		questData[index].isClear = true;
		questListWidget->RemoveList(NPC->GetQuestNumber());
		SpaceShip->RemoveList(NPC->GetQuestNumber());
	}
	QuestButtonVisible();
	NPC->UpdateTalk(this);
}

void APlayer_Human::AddRemoveQuest(uint8 inputQuestNumber, float inputRemoveEnemyNum, float inputRemoveCount, FText inputQuestName)
{
	FQuestStruct tempQuestStruct;
	tempQuestStruct.targetEnemyNum1 = inputRemoveEnemyNum;
	tempQuestStruct.RemoveGoal1 = inputRemoveCount;
	tempQuestStruct.questName = inputQuestName;
	questListWidget->AddList(tempQuestStruct, NPC->GetQuestNumber());
	SpaceShip->AddList(tempQuestStruct, NPC->GetQuestNumber());	//우주선에 따로 있는 퀘스트창에도 업데이트 해줘야함.

	questData.Add(tempQuestStruct);
	receiveQuestNumber.Add(inputQuestNumber);	
}

void APlayer_Human::AddCollectQuest(uint8 inputQuestNumber, float inputCollectItemNum, float inputCollectItemCount, FText inputQuestName)
{
	FQuestStruct tempQuestStruct;
	tempQuestStruct.needItemNum1 = inputCollectItemNum;
	tempQuestStruct.needItemAmount1 = inputCollectItemCount;
	tempQuestStruct.questName = inputQuestName;

	questData.Add(tempQuestStruct);
	receiveQuestNumber.Add(inputQuestNumber);
}

TArray <uint8>  APlayer_Human::GetReceiveQuestNumber()
{
	return receiveQuestNumber;
}

void APlayer_Human::SetVisibleQuestWindow(bool inputOnOff)
{
	if (!inputOnOff)
	{
		questWidget->RemoveFromViewport();
		questWidget->VisibleAcceptBtn(0);
		questWidget->VisibleClearBtn(0);
	}
	else
		questWidget->AddToViewport();	
}


void APlayer_Human::InteractDragonKeyPressed()
{
	isInteractDragonKey = true;
}
void APlayer_Human::InteractDragonKeyReleased()
{
	isInteractDragonKey = false;
}

bool APlayer_Human::GetIscatchDragon()
{
	return isCatchDragon;
}


void APlayer_Human::TutorialBegin()
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) GamePause();		//만약 일시정지 상태이면 튜토리얼 진행시 자동으로 풀림

	if (IsValid(MyController))	MyController->bShowMouseCursor = true;
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	tutorialWidget->AddToViewport();
	isPlanetTutorialDone = true;
}

void APlayer_Human::TutorialEnd()
{
	if (IsValid(MyController))	MyController->bShowMouseCursor = false;
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	tutorialWidget->SetCurrentPage_0();
	tutorialWidget->RemoveFromViewport();
}

bool APlayer_Human::GetIsSpaceTurorialDone()
{
	return isPlanetTutorialDone;
}
void APlayer_Human::SetIsSpaceTurorialDone(bool inputIsHumanTutorialDone)
{
	isPlanetTutorialDone = inputIsHumanTutorialDone;
}

void APlayer_Human::GamePause()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		MyController->SetShowMouseCursor(false);
		Player_HumanUI->GamePauseUIVisible(0);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
	else
	{
		MyController->SetShowMouseCursor(true);
		Player_HumanUI->GamePauseUIVisible(1);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void APlayer_Human::EraseSaveData()
{
	isPlanetTutorialDone = false;
	isCatchDragon = false;
}