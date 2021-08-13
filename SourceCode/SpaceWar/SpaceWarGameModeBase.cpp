// Copyright Epic Games, Inc. All Rights Reserved.


#include "SpaceWarGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include <EngineUtils.h>
#include "GameFramework/PlayerController.h"
#include "PlayerCPP.h"
#include "Player_Human.h"
#include "MySaveGame.h"
#include "Blueprint/UserWidget.h"

ASpaceWarGameModeBase::ASpaceWarGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpaceWarGameModeBase::InitGameState()
{
	Super::InitGameState();

}
void ASpaceWarGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TArray <AActor*> obj;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCPP::StaticClass(), obj);
	for (AActor* actor : obj)
	{
		player_SpaceShip = Cast<APlayerCPP>(actor);

		if (IsValid(player_SpaceShip))
			break;
	}
	
	TArray <AActor*> obj2;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Human::StaticClass(), obj2);
	for (AActor* actor : obj2)
	{
		player_Human = Cast<APlayer_Human>(actor);

		if (IsValid(player_Human))
			break;
	}
	

	if (player_SpaceShip)
	{
		saveData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SavePlayer"), 0));

		if (saveData == nullptr)
		{
			auto saveGame = UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass());
			saveData = Cast<UMySaveGame>(saveGame);

			saveData->currentSpaceShipHp = player_SpaceShip->GetPlayerFullHP();
			saveData->SpaceShipFireRate = player_SpaceShip->GetFireRate();
			saveData->SpaceShipDamage = player_SpaceShip->GetDamage();
			saveData->SpaceShipMaxMagazine = player_SpaceShip->GetMaxMagazine();
			saveData->SpaceShipMaxHp = player_SpaceShip->GetPlayerFullHP();
			saveData->money = 0;
			saveData->upgradeLevel.Init(0, 4);

			SaveData_SpaceShip();
		}
		else
		{
			LoadData_SpaceShip();
		}

	}
	
	isLoadNewLevel = true;


}
void ASpaceWarGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (isLoadNewLevel)
	{
		SettingAfterLoadLevel();
	}

	if (isLoadAnimation)
	{
		LoadLevelAnimation();
	}
	
}


void ASpaceWarGameModeBase::FadeOutEffect(float FadeInOutTime, APlayerController* Controller)
{
	//Controller->ClientSetCameraFade(true, FColor::Black, FVector2D(0.0, 1.0), FadeInOutTime, false, true);
	GetWorld()->GetFirstPlayerController()->ClientSetCameraFade(true, FColor::Black, FVector2D(0.0, 1.0), FadeInOutTime, false, true);
}

void ASpaceWarGameModeBase::FadeInEffect(float FadeInOutTime, APlayerController* Controller)
{
	//Controller->ClientSetCameraFade(true, FColor::Black, FVector2D(1.0, 0.0), FadeInOutTime, false, false);
	GetWorld()->GetFirstPlayerController()->ClientSetCameraFade(true, FColor::Black, FVector2D(1.0, 0.0), FadeInOutTime, false, false);
}

void ASpaceWarGameModeBase::LoadNewLevel(FName LevelName)
{
	NextPlanetName = LevelName;
	isLoadAnimation = true;
}

void ASpaceWarGameModeBase::LoadLevel(FName LevelName)
{
	if (!currentPlayerType)		//현재 플래이어가 우주선일때 우주선 관련 데이터 저장
	{
		SaveData_SpaceShip();
	}
	else if (currentPlayerType)
	{
		SaveData_Human();
	}

	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void ASpaceWarGameModeBase::SaveData_SpaceShip()
{
	if (saveData)
	{		
		saveData->currentSpaceShipHp = player_SpaceShip->GetPlayerHP();		
		saveData->SpaceShipFireRate = player_SpaceShip->GetFireRate();
		saveData->SpaceShipDamage = player_SpaceShip->GetDamage();
		saveData->SpaceShipMaxMagazine = player_SpaceShip->GetMaxMagazine();
		saveData->SpaceShipMaxHp = player_SpaceShip->GetPlayerFullHP();		
		if(player_SpaceShip->GetSpaceSpawnPositon().GetLocation() != FVector(0,0,0)) saveData->spaceSpawnPosition = player_SpaceShip->GetSpaceSpawnPositon();		

		saveData->inventory_ItemNumber = player_SpaceShip->GetInventory_ItemNumber();
		saveData->inventory_ItemAmount = player_SpaceShip->GetInventory_ItemAmount();
		saveData->money = player_SpaceShip->GetMoney();
		saveData->upgradeLevel = player_SpaceShip->GetUpgradeLevel();

		saveData->isSpaceTutorialDone = player_SpaceShip->GetIsSpaceTurorialDone();

		
		UGameplayStatics::SaveGameToSlot(saveData, TEXT("SavePlayer"), 0);
	}
}

void ASpaceWarGameModeBase::LoadData_SpaceShip()
{

	saveData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SavePlayer"), 0));

	player_SpaceShip->SetPlayerHP(saveData->currentSpaceShipHp);
	player_SpaceShip->SetPlayerFullHP(saveData->SpaceShipMaxHp);
	player_SpaceShip->SetFireRate(saveData->SpaceShipFireRate);
	player_SpaceShip->SetDamage(saveData->SpaceShipDamage);
	player_SpaceShip->SetMaxMagazine(saveData->SpaceShipMaxMagazine);

	player_SpaceShip->LoadInventory(saveData->inventory_ItemNumber, saveData->inventory_ItemAmount, saveData->money);
	player_SpaceShip->SetUpgradeLevel(saveData->upgradeLevel);

	player_SpaceShip->SetIsSpaceTurorialDone(saveData->isSpaceTutorialDone);
}

void ASpaceWarGameModeBase::LoadData_Human()
{

	saveData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SavePlayer"), 0));	
	player_Human->LoadInventory(saveData->inventory_ItemNumber, saveData->inventory_ItemAmount, saveData->money);
	player_Human->SetIsSpaceTurorialDone(saveData->isHumanTutorialDone);
	player_Human->isCatchDragon = saveData->isCatchDragon;
}

void ASpaceWarGameModeBase::SaveData_Human()
{
	if (saveData)
	{
		saveData->inventory_ItemNumber = player_Human->GetInventory_ItemNumber();
		saveData->inventory_ItemAmount = player_Human->GetInventory_ItemAmount();
		saveData->money = player_Human->GetMoney();
		saveData->isHumanTutorialDone = player_Human->GetIsSpaceTurorialDone();
		saveData->isCatchDragon = player_Human->GetIscatchDragon();
		UGameplayStatics::SaveGameToSlot(saveData, TEXT("SavePlayer"), 0);
	}
}



void ASpaceWarGameModeBase::SwitchControllerMaster()
{
	player_SpaceShip->GiveUpController();	
}

void ASpaceWarGameModeBase::LoadLevelAnimation()
{
	if (currentDuringDarkScene == 0)	FadeOutEffect(1.5, MyController);

	currentDuringDarkScene += GetWorld()->DeltaTimeSeconds;

	if (currentDuringDarkScene > duringDarkScene + 1)	LoadLevel(NextPlanetName);
}

void ASpaceWarGameModeBase::SettingAfterLoadLevel()
{	

	if (GetWorld()->GetMapName().Contains(TEXT("Space")))
	{
		MyController = player_SpaceShip->GetPlayerController();
		
		FadeInEffect(2, MyController);
		currentPlayerType = 0;
		player_SpaceShip->SetActorTransform(saveData->spaceSpawnPosition);
	}
	else if (GetWorld()->GetMapName().Contains(TEXT("Planet")))
	{
		MyController = player_Human->GetPlayerController();				
		FadeInEffect(2, MyController);
		currentPlayerType = 1;

	}
	else if (GetWorld()->GetMapName().Contains(TEXT("Menu")))
	{
		if (mainmenuWidgetFactory)
		{
			// ReadyUI를 하나 만들고 싶다.
			mainmenuWidget = CreateWidget<UUserWidget>(GetWorld(), mainmenuWidgetFactory);
			//화면에 UI가 보이도록 하기
			mainmenuWidget->AddToViewport();
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor=true;
		}
	}

	isLoadNewLevel = false;

}

