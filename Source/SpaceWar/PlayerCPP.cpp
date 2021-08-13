// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCPP.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include <EngineUtils.h>
#include "PlayerBullet.h"
#include "Enemy_BulletBasic.h"
#include "Enemy_BulletBall.h"
#include "SpaceWarGameModeBase.h"
#include "Planet.h"
#include "Player_Human.h"
#include "Item.h"
#include <Components/WidgetComponent.h>
#include "PlayerHpBar_Widget.h"
#include "Inventory_Widget.h"
#include "UpgradeWidget.h"
#include "ParkZone.h"
#include "Bullet_Missile.h"
#include "SpaceEnemy.h"
#include "QuestWidget.h"
#include "NPC_Parent.h"
#include "QuestListWidget.h"
#include "TutorialWidget.h"

// Sets default values
APlayerCPP::APlayerCPP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = collComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("what is it"));
	springArmComp->SetupAttachment(collComp);

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Sight"));
	cameraComp->SetupAttachment(springArmComp, USpringArmComponent::SocketName);

	springArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	springArmComp->TargetArmLength = 400.f;
	springArmComp->bEnableCameraLag = true;
	springArmComp->CameraLagSpeed = 3.0f;

	firePosition1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position1"));
	firePosition1->SetupAttachment(collComp);

	firePosition2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position2"));
	firePosition2->SetupAttachment(collComp);

	fireMissilePosition1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Missile Position1"));
	fireMissilePosition1->SetupAttachment(collComp);

	fireMissilePosition2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Missile Position2"));
	fireMissilePosition2->SetupAttachment(collComp);

	player_HumanGetOffPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Player Get Off Position"));
	player_HumanGetOffPosition->SetupAttachment(collComp);

	player_HumanChairPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Player Chair Position"));
	player_HumanChairPosition->SetupAttachment(collComp);

	myAimingPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("My AimingPoint"));
	myAimingPoint->SetupAttachment(collComp);

	aimingPointWidgetFactory = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("AimingPoint2"));
	aimingPointWidgetFactory->AttachToComponent(myAimingPoint, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void APlayerCPP::BeginPlay()
{
	Super::BeginPlay();

	gameMode = Cast<ASpaceWarGameModeBase>(GetWorld()->GetAuthGameMode());
	MyController = Cast<APlayerController>(Controller);
	//MyController->Possess(Cast<APawn>(this));
	//currentHp = fullHp;

	currentMagazine = maxMagazine;
	accel = normalModeAccel;
	upgradeLevel.Init(0, 4);

	TArray <AActor*> obj;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Human::StaticClass(), obj);
	player_Human = NULL;
	for (AActor* actor : obj)
	{
		player_Human = Cast<APlayer_Human>(actor);

		if (IsValid(player_Human))
			break;
	}

	if (InteractionWidgetFactory)
	{
		InteractionWidget = CreateWidget<UUserWidget>(GetWorld(), InteractionWidgetFactory);
	}

	if (underAttackWarningFactory)
	{
		underAttackWarning = CreateWidget<UUserWidget>(GetWorld(), underAttackWarningFactory);
		underAttackWarning->AddToViewport();
		underAttackWarning->SetColorAndOpacity(FLinearColor(1, 1, 1, currentUnderAttackOpacity));
	}


	if (PlayerHpBarWidget)
	{
		PlayerHpBar = CreateWidget<UPlayerHpBar_Widget>(GetWorld(), PlayerHpBarWidget);
		PlayerHpBar->SetOwner(this);
		PlayerHpBar->AddToViewport();
	}

	if (InventoryWidgetComp)
	{
		InventoryWidget = CreateWidget<UInventory_Widget>(GetWorld(), InventoryWidgetComp);	
		InventoryWidget->AddToViewport();
		InventoryWidget->SetInventoryVisible(0);
	}
	
	if (upgradeWidgetComp)
	{
		upgradeWidget = CreateWidget<UUpgradeWidget>(GetWorld(), upgradeWidgetComp);
		upgradeWidget->SetOwner(this);
	}

	if (questListWidgetFactory)
	{
		questListWidget = CreateWidget<UQuestListWidget>(GetWorld(), questListWidgetFactory);		
	}

	if (tutorialWidgetFactory)
	{
		tutorialWidget = CreateWidget<UTutorialWidget>(GetWorld(), tutorialWidgetFactory);		
		tutorialWidget->SetOwner(this);
	}

	aimingPointWidget = Cast<UUserWidget>(aimingPointWidgetFactory->GetUserWidgetObject());
	
	

	//로드
	gameMode->LoadData_SpaceShip();
	if (currentHp <= fullHp / 2) currentHp = fullHp / 2;	
	InventoryWidget->LoadInventory(tempInventory_ItemNumber, tempInventory_ItemAmount, tempMoney);

	if (!IsValid(MyController))
	{
		SetUIVisible(0);
		SetPlayer_HumanBoard(false);
	}
	
	UpgradeInit();	

	if (GetWorld()->GetMapName().Contains(TEXT("Space")))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_BoardAShip, GetActorLocation());
		if(!isSpaceTutorialDone) TutorialBegin();
	}

	
}

// Called every frame
void APlayerCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//AutoPossessPlayer = EAutoReceiveInput::Player0;	
	

	if (isPlayer_HumanBoard)
	{
		player_Human->SetActorLocation(player_HumanChairPosition->GetComponentLocation());

		Move();
		Turn();
	}
	
	if (currentHp <= 0)
	{
		Dead();
	}

	if (isReloading) Reloading();

	if (currentFireRate < fireRate) currentFireRate += DeltaTime;

	if (isMissileCoolTime)
	{		
		MissileCoolTimeWork();
	}

	OnCollision_During();

	if (isUnderAttackOpacityWork)
	{
		UnderAttackOpacityWork();
	}

	if (currentMessagePrintTime <= messagePrintTime)
	{
		MessageWork();
	}
	
	QuestCanClear();
	
}

// Called to bind functionality to input
void APlayerCPP::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("CameraPitch", this, &APlayerCPP::PitchCamera);
	PlayerInputComponent->BindAxis("CameraYaw", this, &APlayerCPP::YawCamera);
	PlayerInputComponent->BindAxis("Forward", this, &APlayerCPP::MoveForward);
	PlayerInputComponent->BindAxis("Fire", this, &APlayerCPP::FirePressing);
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerCPP::InputHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerCPP::InputVertical);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCPP::ReloadPress);
	PlayerInputComponent->BindAction("FullHp", IE_Pressed, this, &APlayerCPP::FullHp);
	PlayerInputComponent->BindAction("FlyingMode", IE_Pressed, this, &APlayerCPP::ChangeFlyingMode);
	PlayerInputComponent->BindAction("GetItem", IE_Pressed, this, &APlayerCPP::GetItemPressed);
	//PlayerInputComponent->BindAction("GetItem", IE_Released, this, &APlayerCPP::GetItemReleased);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &APlayerCPP::InventoryVisible);
	PlayerInputComponent->BindAction("UpgradeWindow", IE_Pressed, this, &APlayerCPP::UpgradeWindowVisible);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &APlayerCPP::InteractionEvent);
	PlayerInputComponent->BindAction("FireMissile", IE_Pressed, this, &APlayerCPP::FireMissile);
	PlayerInputComponent->BindAction("QuestList", IE_Pressed, this, &APlayerCPP::VisibleQuestList);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCPP::GamePause);
	

	PlayerInputComponent->BindAction("SaveDataErase", IE_Pressed, this, &APlayerCPP::EraseSaveData);
}

void APlayerCPP::PitchCamera(float value)
{
	if (!isAbleControl)	return;
	if (MyController->ShouldShowMouseCursor())
	{
		aimingPointInput.Y = 0;
		return;
	}

	//TurnPitchValue = value/10;
	//CameraInput.Y = value;
	aimingPointInput.Y = value * 0.8;
	
}

void APlayerCPP::YawCamera(float value)
{
	if (!isAbleControl)	return;	
	if (MyController->ShouldShowMouseCursor())
	{
		aimingPointInput.X = 0;
		return;
	}

//	TurnYawValue = value / 10;
	//CameraInput.X = value;
	aimingPointInput.X = value * 0.8;
	
}

void APlayerCPP::Turn()
{
	if (isAbleControl)
	{
		if (Mode == 1)
		{
			FRotator NewRotation3 = myAimingPoint->GetRelativeRotation();
			NewRotation3.Yaw += aimingPointInput.X;
			NewRotation3.Pitch += aimingPointInput.Y;
			NewRotation3.Roll = 0;
			if (abs(NewRotation3.Pitch) > 30) NewRotation3.Pitch = (abs(NewRotation3.Pitch) / NewRotation3.Pitch) * 30;
			if (abs(NewRotation3.Yaw) > 30) NewRotation3.Yaw = (abs(NewRotation3.Yaw) / NewRotation3.Yaw) * 30;

			myAimingPoint->SetRelativeRotation(NewRotation3);
			firePosition1->SetRelativeRotation(NewRotation3);
			firePosition2->SetRelativeRotation(NewRotation3);
			
			//FRotator NewRotation2 = myAimingPoint->GetComponentRotation();
			

			FRotator NewRotation = GetActorRotation();

			//NewRotation.Yaw += ((NewRotation3.Yaw / 30) * 1);
			//NewRotation.Pitch += ((NewRotation3.Pitch / 30) * 1);
			
			if(NewRotation3.Yaw > 0) NewRotation.Yaw += (FMath::Pow(3, (NewRotation3.Yaw / 30))-1);
			else NewRotation.Yaw -= (FMath::Pow(3, (-NewRotation3.Yaw / 30)) - 1);

			
			if (NewRotation3.Pitch > 0) NewRotation.Pitch += (FMath::Pow(3, (NewRotation3.Pitch / 30))-1);
			else  NewRotation.Pitch -= (FMath::Pow(3, (-NewRotation3.Pitch / 30)) - 1);

			//NewRotation.Yaw += CameraInput.X;
			//NewRotation.Pitch += CameraInput.Y;

			if (abs(NewRotation.Pitch) > 80) NewRotation.Pitch = (abs(NewRotation.Pitch) / NewRotation.Pitch) * 80;			
			//NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 80.0f);

			NewRotation.Roll += 0;

			SetActorRotation(NewRotation);
		}
		else
		{

		}
	}

}

void APlayerCPP::InputHorizontal(float value)
{
	FRotator newRotation = GetActorRotation();
	newRotation.Pitch += 0;
	newRotation.Yaw += 0;
	currentRoll = GetActorRotation().Roll;
	float turnRollacel = 0.01;

	if (isAbleControl)
	{
		if (Mode == 1)
		{
			if (!FlyingMode)
			{
				if (value == 0 && abs(currentHorizontalSpeed) > 1)
				{
					currentHorizontalSpeed += (currentHorizontalSpeed / abs(currentHorizontalSpeed)) * (-50);
				}

				if (value == 1)
				{
					if (currentHorizontalSpeed > horizontalMaxspeed)
					{
						currentHorizontalSpeed = horizontalMaxspeed;
					}
					else
					{
						currentHorizontalSpeed += 50;
					}

				}
				else if (value == -1)
				{
					if (currentHorizontalSpeed < -horizontalMaxspeed)
					{
						currentHorizontalSpeed = -horizontalMaxspeed;
					}
					else
					{
						currentHorizontalSpeed += -50;
					}
				}

				dirHorizon = GetActorRightVector();
				dirHorizon.Z = 0;



				if (value < 0)
				{

					if (currentRoll <= -20)
					{
						turnRollSpeed = 0;
					}
					else if (currentRoll < -10)
					{
						turnRollSpeed += turnRollacel;
					}
					else if (currentRoll >= -10)
					{
						turnRollSpeed -= turnRollacel;
					}

				}
				else if (value > 0)
				{

					if (currentRoll >= 20)
					{
						turnRollSpeed = 0;
					}
					else if (currentRoll > 10)
					{
						turnRollSpeed -= turnRollacel;
					}
					else if (currentRoll <= 10)
					{
						turnRollSpeed += turnRollacel;
					}
				}
				else if (value == 0)
				{
					if (currentRoll < 0)
					{
						turnRollSpeed += turnRollacel;
						if (turnRollSpeed >= 0.2)
						{
							turnRollSpeed = 0.2;
						}
					}
					else if (currentRoll > 0)
					{
						turnRollSpeed -= turnRollacel;
						if (turnRollSpeed <= -0.2)
						{
							turnRollSpeed = -0.2;
						}
					}
					else
					{
						turnRollSpeed = 0;
					}
				}

				newRotation.Roll += turnRollSpeed;
				if (abs(newRotation.Roll) < 0.3 && value == 0)
				{
					newRotation.Roll = 0;
					turnRollSpeed = 0;
				}
				SetActorRotation(newRotation);



			}
			else
			{
				if (value == 0)
				{
					if (currentRoll < 0)
					{
						turnRollSpeed += turnRollacel;
						if (turnRollSpeed >= 0.2)
						{
							turnRollSpeed = 0.2;
						}
					}
					else if (currentRoll > 0)
					{
						turnRollSpeed -= turnRollacel;
						if (turnRollSpeed <= -0.2)
						{
							turnRollSpeed = -0.2;
						}
					}
					else
					{
						turnRollSpeed = 0;
					}
				}

				newRotation.Roll += turnRollSpeed;
				if (abs(newRotation.Roll) < 0.5 && value == 0)
				{
					newRotation.Roll = 0;
					turnRollSpeed = 0;
				}
				SetActorRotation(newRotation);
			}


			return;
		}
		else
		{

		}

		SetActorRotation(GetActorRotation() + FRotator(0, currentTurnValue_Horizontal, 0));
	}

}
void APlayerCPP::InputVertical(float value)
{
	if (isAbleControl)
	{
		if (Mode == 1)
		{
			return;
		}
		
	}
	else
	{

	}
}
void APlayerCPP::FullHp()
{
	currentHp = fullHp;
}

void APlayerCPP::MoveForward(float value)
{

	dir = GetActorForwardVector();


	if (value == 1)
	{
		dir *= value;

		currentSpeed += accel;
		if (currentSpeed > Maxspeed) currentSpeed = Maxspeed;
	}
	else if (value == -1)
	{

		currentSpeed -= accel;
		if (currentSpeed < backSpeed) currentSpeed = backSpeed;
	}
	else if (value == 0 && currentSpeed != 0)
	{
		currentSpeed -= decel;
		if (currentSpeed < 0) currentSpeed = 0;
	}

	dir.Normalize();


}

void APlayerCPP::Move()
{

	dir = GetActorForwardVector();
	//currentSpeed = Maxspeed;

	FVector P0 = GetActorLocation();
	FVector vel = dir * GetWorld()->DeltaTimeSeconds * currentSpeed;
	FVector P = P0 + vel;

	FVector vel2 = dirHorizon * GetWorld()->DeltaTimeSeconds * currentHorizontalSpeed;
	FVector P2 = P0 + vel2 + vel;

	SetActorLocation(P2, true);

}

void APlayerCPP::Fire()
{
	
	RandomFireVoid();

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APlayerBullet* bullet1 = GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, firePosition1->GetComponentTransform(), param);
	APlayerBullet* bullet2 = GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, firePosition2->GetComponentTransform(), param);

	if (IsValid(bullet1) && IsValid(bullet2))
	{
		bullet1->SetBulletDamage(currentBulletDamage);
		bullet2->SetBulletDamage(currentBulletDamage);

		bullet1->SetBulletOwner(this);
		bullet2->SetBulletOwner(this);
	}

}

void APlayerCPP::FireMissile()
{
	if (isMissileCoolTime) return;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), missileSound, GetActorLocation());
	RandomFireVoid();

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	
	bool isThereHomingTarget = false;
	TArray <AActor*> obj;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpaceEnemy::StaticClass(), obj);	
	ASpaceEnemy* Enemy = NULL;

	float distance;
	for (AActor* actor : obj)
	{
		Enemy = Cast<ASpaceEnemy>(actor);
		
		if (IsValid(Enemy))
		{
			distance = GetDistanceTo(Enemy);
			
			if (distance < 50000)
			{
				isThereHomingTarget = true;
				break;
			}			
		}			
	}

	for (int i = 0; i < 10; i++)
	{		

		float spread_Pitch = FMath::RandRange(-missileSpread, missileSpread);
		float spread_Yaw = FMath::RandRange(-missileSpread, missileSpread);
		float spread_Pitch2 = FMath::RandRange(-missileSpread, missileSpread);
		float spread_Yaw2 = FMath::RandRange(-missileSpread, missileSpread);

		ABullet_Missile* bullet1 = GetWorld()->SpawnActor<ABullet_Missile>(MissileFactory, fireMissilePosition1->GetComponentLocation(), fireMissilePosition1->GetComponentRotation() + FRotator(spread_Pitch, spread_Yaw,0), param);
		ABullet_Missile* bullet2 = GetWorld()->SpawnActor<ABullet_Missile>(MissileFactory, fireMissilePosition2->GetComponentLocation(), fireMissilePosition2->GetComponentRotation() + FRotator(spread_Pitch2, spread_Yaw2, 0), param);
				

		if (IsValid(bullet1) && IsValid(bullet2))
		{
			bullet1->SetBulletOwner(this);
			bullet2->SetBulletOwner(this);

			if (isThereHomingTarget)
			{
				bullet1->SetLifeTime(3 + FMath::RandRange(-0.5f, 0.5f), false);
				bullet2->SetLifeTime(3 + FMath::RandRange(-0.5f, 0.5f), false);

				bullet1->SetAutoTargetHomingMissile(FMath::RandRange(0.1f, 2.0f));
				bullet2->SetAutoTargetHomingMissile(FMath::RandRange(0.1f, 2.0f));
			}
			else
			{

				bullet1->SetLifeTime(FMath::RandRange(2.0f, 3.0f), false);
				bullet2->SetLifeTime(FMath::RandRange(2.0f, 3.0f), false);

				FVector spawnLocation1 = (GetActorForwardVector() * 50000) + GetActorLocation();
				spawnLocation1.Y += FMath::RandRange(-5000, 5000);
				spawnLocation1.Z += FMath::RandRange(-5000, 5000);

				FVector spawnLocation2 = (GetActorForwardVector() * 50000) + GetActorLocation();
				spawnLocation2.Y += FMath::RandRange(-5000, 5000);
				spawnLocation2.Z += FMath::RandRange(-5000, 5000);

				bullet1->SetHomingMissile(spawnLocation1, 1);
				bullet2->SetHomingMissile(spawnLocation2, 1);
			}	
		}
	}

	isMissileCoolTime = true;
	currentMissileCoolTime = 0;
}

void APlayerCPP::MissileCoolTimeWork()
{
	currentMissileCoolTime += GetWorld()->DeltaTimeSeconds;

	if (currentMissileCoolTime >= missileCoolTime)
	{		
		isMissileCoolTime = false;
		PlayerHpBar->SetMissileCoolTimeDoneWork();
	}
}

void APlayerCPP::RandomFireVoid()
{
	if (FMath::RandRange(1, 100) < voice_FireProbability)
	{
		switch (FMath::RandRange(1, 3))
		{
		case 1:
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_Fire1, GetActorLocation());
			break;
		case 2:
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_Fire2, GetActorLocation());
			break;
		case 3:
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_Fire3, GetActorLocation());
			break;
		}

	}
}

void APlayerCPP::Dead()
{	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_Dead, GetActorLocation());
	SetUIVisible(0);
	gameMode->LoadNewLevel("TheSpace");
	Destroy();	
}

void APlayerCPP::OnCollisionEnter_Overlab(AActor* OtherActor)
{
	AEnemyBullet_Parent* enemyBullet = Cast<AEnemyBullet_Parent>(OtherActor);
	APlanet* planet = Cast<APlanet>(OtherActor);	
	AParkZone* parkZone = Cast<AParkZone>(OtherActor);
	AItem* itemTemp = Cast<AItem>(OtherActor);

	if (IsValid(enemyBullet))
	{
		SetMessage("We're under attack.");
		RandomImHitSound();

		SetUnderAttackOpacityWork();
		currentHp -= enemyBullet->GetBulletDamage();
		enemyBullet->ExploreEffect();
		enemyBullet->Destroy();
	}
	else if (IsValid(planet))
	{
		SetMessage("Approching the planet.");		

		PlayerHpBar->SetAimingPointVisible(2);
		FName planetName = planet->GetPlanetName();
		spaceSpawnPosition = planet->spawnPosition->GetComponentTransform();		
		isAbleControl = false;
		gameMode->LoadNewLevel(planetName);
	}
	else if (IsValid(parkZone))
	{
		isCanGetOff = true;
		isInteraction = true;
		parkTransform = parkZone->parkPosition->GetComponentTransform();
		InteractionWidgetRemoveAdd(1);
	}
	else if(IsValid(itemTemp))
	{
		
		isInteraction = true;
		
		Item.Add(itemTemp);
		InteractionWidgetRemoveAdd(1);
		ItemInteractionCount++;		
		
	}

}

void APlayerCPP::OnCollision_End(AActor* OtherActor)
{
	if (OtherActor != this)
	{
		AItem* tempItem = Cast<AItem>(OtherActor);
		

		if (IsValid(tempItem))
		{
			Item.Remove(tempItem);
			ItemInteractionCount--;
			if(ItemInteractionCount == 0)
			{
				InteractionWidgetRemoveAdd(0);
				isInteraction = false;
			}
	
		}
		else if(isCanGetOff)
		{
			isCanGetOff = false;
			InteractionWidgetRemoveAdd(0);
			isInteraction = false;
		}
	}
	
	
}

void APlayerCPP::OnCollision_During()
{
	
	if (isInteraction)
	{
		if (isPicked)
		{
			PickUpItem();
					
		}

	}
}

void APlayerCPP::TakeDamage(float damage)
{
	currentHp -= damage;
	if(currentHp > 0 )RandomImHitSound();		
	SetUnderAttackOpacityWork();
}

void APlayerCPP::RandomImHitSound()
{
	if (FMath::RandRange(1, 100) < voice_ImHitProbability)
	{
		switch (FMath::RandRange(1, 3))
		{
		case 1:
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_ImHit1, GetActorLocation());
			break;
		case 2:
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_ImHit2, GetActorLocation());
			break;
		case 3:
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_ImHit3, GetActorLocation());
			break;

		}

	}
}

void APlayerCPP::RandomVictorySound()
{

	if (FMath::RandRange(1, 100) < voice_RemoveEnemyProbability)
	{
		switch (FMath::RandRange(1, 3))
		{
		case 1:
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_RemoveEnemy1, GetActorLocation());
			break;
		case 2:
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_RemoveEnemy2, GetActorLocation());
			break;
		case 3:
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_RemoveEnemy3, GetActorLocation());
			break;

		}

	}
}


void APlayerCPP::GetItemPressed()
{
	if (ItemInteractionCount != 0)
	{
		isPicked = true;
	}
}

void APlayerCPP::GetItemReleased()
{
	isPicked = false;
}

void APlayerCPP::InteractionWidgetRemoveAdd(bool RemoveAdd)
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

void APlayerCPP::FirePressing(float value)
{

	if (value && !isReloading && !FlyingMode)
	{
		FireWork();
	}
}

void APlayerCPP::FireWork()
{

	if (currentMagazine > 0 && currentFireRate >= fireRate)
	{

		Fire();
		currentMagazine -= 1;
		currentFireRate = 0;

		if (currentMagazine <= 0) isReloading = true;
	}
}

void APlayerCPP::Reloading()
{
	currentReloadingDelay += GetWorld()->DeltaTimeSeconds;
	
	PlayerHpBar->SetReloadBarVisible(true);

	if (currentReloadingDelay > reloadingDelay)
	{
		currentReloadingDelay = 0;
		currentMagazine = maxMagazine;
		PlayerHpBar->SetReloadBarVisible(false);
		isReloading = false;
	}

}

void APlayerCPP::ReloadPress()
{
	if (!isReloading && currentMagazine < maxMagazine) isReloading = true;



}

void APlayerCPP::ExploreEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exploreFactory, GetActorLocation(), GetActorRotation(), GetActorScale3D());
}




APlayerController* APlayerCPP::GetPlayerController()
{
	return MyController;
}

void APlayerCPP::InteractionEvent()
{
	if (isCanGetOff)
	{		
		GetOffSpaceShip();
	}
	else if (isCanInteractionNPC)
	{
		//그 NPC에 빙의할것. 오늘 볼것임
	}

}

void APlayerCPP::GetOffSpaceShip()
{
	SetActorLocation(parkTransform.GetLocation());
	SetActorRotation(parkTransform.GetRotation());
	currentSpeed = 0;
	currentRoll = 0;
	player_Human->QuestSynchronized(questData, receiveQuestNumber);
	GiveUpController();
}

void APlayerCPP::GiveUpController()
{
	if (IsValid(player_Human) && GetPlayer_HumanBoard())
	{				
		SetPlayer_HumanBoard(false);
		MyController->UnPossess();
		MyController->Possess(player_Human);
		player_Human->SetController();
		player_Human->SetUIVisible(1);		
		player_Human->SetActorLocation(player_HumanGetOffPosition->GetComponentLocation());
		player_Human->SetActorRotation(player_HumanGetOffPosition->GetComponentRotation());
		SetUIVisible(0);
	}
}

void APlayerCPP::SetController()
{
	MyController = GetWorld()->GetFirstPlayerController();
}


void APlayerCPP::SetPlayer_HumanBoard(bool isBoard)
{
	isPlayer_HumanBoard = isBoard;
	if (isBoard) UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_BoardAShip, GetActorLocation());
}

bool APlayerCPP::GetPlayer_HumanBoard()
{
	return isPlayer_HumanBoard;
}

void APlayerCPP::ChangeFlyingMode()
{
	FlyingMode = !FlyingMode;

	if (FlyingMode)
	{
		Maxspeed = Maxspeed_SpeedMode;
		accel = speedModeAccel;
	}
	else
	{
		Maxspeed = Maxspeed_NormalMode;
		accel = normalModeAccel;
		if (currentSpeed > Maxspeed)
		{
			currentSpeed = Maxspeed;
		}

	}
	
	PlayerHpBar->SetAimingPointVisible(FlyingMode);
}

FTransform APlayerCPP::GetSpaceSpawnPositon()
{
	return spaceSpawnPosition;
}

void APlayerCPP::UnderAttackOpacityWork()
{

	if (currentUnderAttackVisualTime > underAttackVisualTime)
	{
		currentUnderAttackOpacity -= underAttackOpacitySpeed;

		if (currentUnderAttackOpacity <= 0)
		{
			currentUnderAttackVisualTime = 0;
			isUnderAttackOpacityWork = false;
		}
	}
	else
	{
		currentUnderAttackVisualTime += GetWorld()->DeltaTimeSeconds;
	}


	underAttackWarning->SetColorAndOpacity(FLinearColor(1, 1, 1, currentUnderAttackOpacity));

}

void APlayerCPP::SetUnderAttackOpacityWork()
{
	isUnderAttackOpacityWork = true;
	currentUnderAttackVisualTime = 0;
	currentUnderAttackOpacity = 1;
}

void APlayerCPP::SetUIVisible(bool OffOn)
{
	if (!OffOn)
	{
		PlayerHpBar->SetVisibility(ESlateVisibility::Hidden);
		PlayerHpBar->SetAimingPointVisible(2);
		InteractionWidget->SetVisibility(ESlateVisibility::Hidden);
		questListWidget->SetVisibleQuestList(0);
		aimingPointWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		PlayerHpBar->SetVisibility(ESlateVisibility::Visible);	
		PlayerHpBar->SetAimingPointVisible(FlyingMode);
		if(isInteraction) InteractionWidget->SetVisibility(ESlateVisibility::Visible);				
		aimingPointWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerCPP::InventoryVisible()
{

	if (InventoryWidget->IsVisible())
		InventoryWidget->SetInventoryVisible(0);
	else
		InventoryWidget->SetInventoryVisible(1);
}

TArray <uint8> APlayerCPP::GetInventory_ItemNumber()
{
	return InventoryWidget->GetInventory_ItemNumber();
}

TArray <uint8> APlayerCPP::GetInventory_ItemAmount()
{
	return InventoryWidget->GetInventory_ItemAmount();
}

int APlayerCPP::GetMoney()
{
	return InventoryWidget->GetMoney();
}

void APlayerCPP::LoadInventory(TArray <uint8> inputItemNumber, TArray <uint8> inputItemAmount, int inputmoney)
{
	tempInventory_ItemNumber = inputItemNumber;
	tempInventory_ItemAmount = inputItemAmount;	
	tempMoney = inputmoney;
}

void APlayerCPP::PickUpItem()
{
	if (IsValid(InventoryWidget))
	{
		InventoryWidget->AddInventory(Item[0]->GetItemNumber());
	}
	isPicked = false;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), pickUpItem, GetActorLocation());
	Item[0]->Destroy();
}

void APlayerCPP::MessageWork()
{
	currentMessagePrintTime += GetWorld()->DeltaTimeSeconds;

	if (currentMessagePrintTime > messagePrintTime)
	{
		SetMessage("");
	}

}

void APlayerCPP::SetMessage(FString inputString)
{
	PlayerHpBar->SetMessage(inputString);
	currentMessagePrintTime = 0;
}

void APlayerCPP::QuestCanClear()
{

	uint8 count = 0;
	for (FQuestStruct quest : questData)
	{

		if ((!questData[count].isCanClear) && (questData[count].RemoveGoal1 == 0) && (questData[count].RemoveGoal2 == 0) && (questData[count].needItemAmount1 == 0) && (questData[count].needItemAmount2 == 0))
		{
			questData[count].isCanClear = true;
			player_Human->QuestSynchronized(questData, receiveQuestNumber);
			
		}
		count++;
	}
}

void APlayerCPP::AddRemoveQuest(uint8 inputQuestNumber ,float inputRemoveEnemyNum, float inputRemoveCount)
{
	FQuestStruct tempQuestStruct;
	tempQuestStruct.targetEnemyNum1 = inputRemoveEnemyNum;
	tempQuestStruct.RemoveGoal1 = inputRemoveCount;

	questData.Add(tempQuestStruct);
	receiveQuestNumber.Add(inputQuestNumber);
}

void APlayerCPP::AddCollectQuest(uint8 inputQuestNumber, float inputCollectItemNum, float inputCollectItemCount)
{
	FQuestStruct tempQuestStruct;
	tempQuestStruct.needItemNum1 = inputCollectItemNum;
	tempQuestStruct.needItemAmount1 = inputCollectItemCount;

	questData.Add(tempQuestStruct);
	receiveQuestNumber.Add(inputQuestNumber);
}

TArray <uint8>  APlayerCPP::GetReceiveQuestNumber()
{
	return receiveQuestNumber;
}

void APlayerCPP::QuestSynchronized(TArray <FQuestStruct> inputQuestData, TArray <uint8> inputreceiveQuestNumber)
{
	questData = inputQuestData;
	receiveQuestNumber = inputreceiveQuestNumber;
}

void APlayerCPP::VisibleQuestList()
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

void APlayerCPP::AddList(FQuestStruct inputquestData, uint8 inputQuestNumber)
{
	questListWidget->AddList(inputquestData, inputQuestNumber);
}

void APlayerCPP::RemoveList(int32 inputQuestNumber)
{
	questListWidget->RemoveList(inputQuestNumber);
}

void APlayerCPP::ListSort()
{
	questListWidget->ListSort();
}

void APlayerCPP::UpgradeWindowVisible()
{
	/*
	FQuestStruct tempQuestStruct;
	tempQuestStruct.targetEnemyNum1 = 1;
	tempQuestStruct.RemoveGoal1 = 3;

	questData.Add(tempQuestStruct);

	*/
	if (upgradeWidget->IsInViewport())
	{
		upgradeWidget->RemoveFromViewport();
		MyController->bShowMouseCursor = false;		
	}
	else
	{
		upgradeWidget->AddToViewport();
		MyController->bShowMouseCursor = true;

	}
}

void APlayerCPP::UpgradeInit()
{
	if(upgradeLevel[0] > 5) upgradeWidget->SetNeedUpgradeIcon(0, -1, 0, 0, 0, true);
	else upgradeWidget->SetNeedUpgradeIcon(0, 4, 1 + FMath::RoundToInt(upgradeLevel[0] * 1.5), 3, 2 + FMath::RoundToInt(upgradeLevel[0] * 1.5));

	if (upgradeLevel[1] > 5) upgradeWidget->SetNeedUpgradeIcon(1, -1, 0, 0, 0, true);
	else upgradeWidget->SetNeedUpgradeIcon(1, 1, 2 + FMath::RoundToInt(upgradeLevel[1] * 1.5), 2, 2 + FMath::RoundToInt(upgradeLevel[1] * 1.5));

	if (upgradeLevel[2] > 5)  upgradeWidget->SetNeedUpgradeIcon(2, -1, 0, 0, 0, true);
	else upgradeWidget->SetNeedUpgradeIcon(2, 1, 2 + FMath::RoundToInt(upgradeLevel[2] * 1.5), 3, 2 + FMath::RoundToInt(upgradeLevel[2] * 1.5));

	if (upgradeLevel[3] > 10) upgradeWidget->SetNeedUpgradeIcon(3, -1, 0, 0, 0, true);
	else upgradeWidget->SetNeedUpgradeIcon(3, 1, 2 + FMath::RoundToInt(upgradeLevel[3] * 1.5), 2, 2 + FMath::RoundToInt(upgradeLevel[3] * 1.5));
}




/// upgrade
void APlayerCPP::UpgradeDamage()
{
	uint8 needItemNum1 = 4;
	uint8 needItemAmount1 = 1 + FMath::RoundToInt(upgradeLevel[0] * 1.5);
	uint8 needItemNum2 = 3;
	uint8 needItemAmount2 = 2 + FMath::RoundToInt(upgradeLevel[0] * 1.5);

	if (upgradeLevel[0] > 5)
	{
		SetMessage("Upgrade level is max");
	}
	else if ( (InventoryWidget->GetItemAmount(needItemNum1) >= needItemAmount1 ) && (InventoryWidget->GetItemAmount(needItemNum2) >= needItemAmount2) )
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_Upgrade, GetActorLocation());
		SetMessage("Upgrade complete.");
		currentBulletDamage += 2;

		InventoryWidget->RemoveInventory(needItemNum1, needItemAmount1);
		InventoryWidget->RemoveInventory(needItemNum2, needItemAmount2);

		upgradeLevel[0]++;
		
		if(upgradeLevel[0] == 6) upgradeWidget->SetNeedUpgradeIcon(0, -1,0,0,0 , true);
		else upgradeWidget->SetNeedUpgradeIcon(0, needItemNum1, 1 + FMath::RoundToInt(upgradeLevel[0] * 1.5), needItemNum2, 2 + FMath::RoundToInt(upgradeLevel[0] * 1.5));
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_ICant, GetActorLocation());
		SetMessage("Not enought item.");
	}


}

void APlayerCPP::UpgradeFireRate()	//위젯 블루프린트에서 버튼 누르면 호출함
{
	uint8 needItemNum1 = 1;
	uint8 needItemAmount1 = 2 + FMath::RoundToInt(upgradeLevel[1] * 1.5);
	uint8 needItemNum2 = 2;
	uint8 needItemAmount2 = 2 + FMath::RoundToInt(upgradeLevel[1] * 1.5);

	
	if (upgradeLevel[1] > 5)
	{
		SetMessage("Upgrade level is max");
	}
	else if ((InventoryWidget->GetItemAmount(needItemNum1) >= needItemAmount1) && (InventoryWidget->GetItemAmount(needItemNum2) >= needItemAmount2))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_Upgrade, GetActorLocation());
		SetMessage("Upgrade complete.");
		fireRate -= 0.05;

		InventoryWidget->RemoveInventory(needItemNum1, needItemAmount1);
		InventoryWidget->RemoveInventory(needItemNum2, needItemAmount2);

		upgradeLevel[1]++;

		if (upgradeLevel[1] == 6) upgradeWidget->SetNeedUpgradeIcon(1, -1, 0, 0, 0, true);
		else upgradeWidget->SetNeedUpgradeIcon(1, needItemNum1, 2 + FMath::RoundToInt(upgradeLevel[1] * 1.5), needItemNum2, 2 + FMath::RoundToInt(upgradeLevel[1] * 1.5));
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_ICant, GetActorLocation());
		SetMessage("Not enought item.");
	}
}

void APlayerCPP::UpgradeMaxMagazine()
{
	uint8 needItemNum1 = 1;
	uint8 needItemAmount1 = 2 + FMath::RoundToInt(upgradeLevel[2] * 1.5);
	uint8 needItemNum2 = 3;
	uint8 needItemAmount2 = 2 + FMath::RoundToInt(upgradeLevel[2] * 1.5);

	if (upgradeLevel[2] > 5)
	{
		SetMessage("Upgrade level is max");
	}
	else if ((InventoryWidget->GetItemAmount(needItemNum1) >= needItemAmount1) && (InventoryWidget->GetItemAmount(needItemNum2) >= needItemAmount2))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_Upgrade, GetActorLocation());
		SetMessage("Upgrade complete.");
		maxMagazine += 5;
		upgradeLevel[2]++;

		InventoryWidget->RemoveInventory(needItemNum1, needItemAmount1);
		InventoryWidget->RemoveInventory(needItemNum2, needItemAmount2);

		if (upgradeLevel[2] == 6) upgradeWidget->SetNeedUpgradeIcon(2, -1, 0, 0, 0, true);
		else upgradeWidget->SetNeedUpgradeIcon(2, needItemNum1, 2 + FMath::RoundToInt(upgradeLevel[2] * 1.5), needItemNum2, 2 + FMath::RoundToInt(upgradeLevel[2] * 1.5));
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_ICant, GetActorLocation());
		SetMessage("Not enought item.");
	}
}

void APlayerCPP::UpgradeMaxHp()
{
	uint8 needItemNum1 = 1;
	uint8 needItemAmount1 = 2 + FMath::RoundToInt(upgradeLevel[3] * 1.2);
	uint8 needItemNum2 = 2;
	uint8 needItemAmount2 = 2 + FMath::RoundToInt(upgradeLevel[3] * 1.2);

	if (upgradeLevel[3] > 10)
	{
		SetMessage("Upgrade level is max");
	}
	else if ((InventoryWidget->GetItemAmount(needItemNum1) >= needItemAmount1) && (InventoryWidget->GetItemAmount(needItemNum2) >= needItemAmount2))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_Upgrade, GetActorLocation());
		SetMessage("Upgrade complete.");
		fullHp += 50;
		upgradeLevel[3]++;

		InventoryWidget->RemoveInventory(needItemNum1, needItemAmount1);
		InventoryWidget->RemoveInventory(needItemNum2, needItemAmount2);

		if (upgradeLevel[3] == 11) upgradeWidget->SetNeedUpgradeIcon(3, -1, 0, 0, 0, true);
		else upgradeWidget->SetNeedUpgradeIcon(3, needItemNum1, 2 + FMath::RoundToInt(upgradeLevel[3] * 1.2), needItemNum2, 2 + FMath::RoundToInt(upgradeLevel[3] * 1.2));
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_ICant, GetActorLocation());
		SetMessage("Not enought item.");
	}
	
}

void APlayerCPP::UpgradeCurrentHp()
{
	uint8 needItemNum1 = 2;
	uint8 needItemAmount1 = 1;


	if (InventoryWidget->GetItemAmount(needItemNum1) >= needItemAmount1)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_Upgrade, GetActorLocation());
		SetMessage("Upgrade complete.");

		InventoryWidget->RemoveInventory(needItemNum1, needItemAmount1);

		currentHp += 30;
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), voice_ICant, GetActorLocation());
		SetMessage("Not enought item.");
	}
}

void APlayerCPP::TutorialBegin()
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) GamePause();		//만약 일시정지 상태이면 튜토리얼 진행시 자동으로 풀림

	if (IsValid(MyController))	MyController->bShowMouseCursor = true;
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	tutorialWidget->AddToViewport();
	isSpaceTutorialDone = true;
}

void APlayerCPP::TutorialEnd()
{
	

	if (IsValid(MyController))	MyController->bShowMouseCursor = false;
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	tutorialWidget->SetCurrentPage_0();
	tutorialWidget->RemoveFromViewport();
}

void APlayerCPP::GamePause()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		MyController->SetShowMouseCursor(false);
		PlayerHpBar->GamePauseUIVisible(0);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}		
	else
	{
		MyController->SetShowMouseCursor(true);
		PlayerHpBar->GamePauseUIVisible(1);
		UGameplayStatics::SetGamePaused(GetWorld(), true);		
	}		
}



	float APlayerCPP::GetPlayerHP()
	{
		return currentHp;
	}
	void APlayerCPP::SetPlayerHP(float InputHP)
	{
		currentHp = InputHP;
	}
	float APlayerCPP::GetPlayerFullHP()
	{
		return fullHp;
	}
	void APlayerCPP::SetPlayerFullHP(float inputValue)
	{
		fullHp = inputValue;
	}
	float APlayerCPP::GetCurrentMagazine()
	{
		return currentMagazine;
	}
	float APlayerCPP::GetMaxMagazine()
	{
		return maxMagazine;
	}
	void APlayerCPP::SetMaxMagazine(float inputValue)
	{
		maxMagazine = inputValue;
	}
	float APlayerCPP::GetFireRate()
	{
		return fireRate;
	}
	void APlayerCPP::SetFireRate(float inputValue)
	{
		fireRate = inputValue;
	}
	float APlayerCPP::GetDamage()
	{
		return currentBulletDamage;
	}
	void APlayerCPP::SetDamage(float inputValue)
	{
		currentBulletDamage = inputValue;
	}
	float APlayerCPP::GetPlayerCurrentReloadTime()
	{
		return currentReloadingDelay;
	}
	float APlayerCPP::GetPlayerReloadTime()
	{
		return reloadingDelay;
	}
	TArray <uint8> APlayerCPP::GetUpgradeLevel()
	{
		return upgradeLevel;
	}
	void APlayerCPP::SetUpgradeLevel(TArray <uint8> inputValue)
	{
		upgradeLevel = inputValue;
	}
	float APlayerCPP::GetMissileCoolTime()
	{
		return missileCoolTime;
	}
	float APlayerCPP::GetCurrentMissileCoolTime()
	{
		return currentMissileCoolTime;
	}
	bool APlayerCPP::GetIsSpaceTurorialDone()
	{
		return isSpaceTutorialDone;
	}
	void APlayerCPP::SetIsSpaceTurorialDone(bool inputIsSpaceTutorialDone)
	{
		isSpaceTutorialDone = inputIsSpaceTutorialDone;
	}

void APlayerCPP::EraseSaveData()
{
	
	SetPlayerFullHP(100);
	SetPlayerHP(GetPlayerFullHP());
	SetMaxMagazine(10);
	SetFireRate(0.5);
	SetDamage(1);
	upgradeLevel.Init(0, 4);
	
	isSpaceTutorialDone = false;
}