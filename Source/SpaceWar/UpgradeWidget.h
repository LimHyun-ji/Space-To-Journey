// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCPP.h"
#include "UpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual bool Initialize();

public:

	TWeakObjectPtr<APlayerCPP> Owner;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* upgradeFireRateBtn;	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* upgradeMaxHpBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* upgradecurrentHpBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* upgradeMaxMagazineBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* upgradeDamageBtn;		

	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnClickedUpgradeFireRateBtn();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnClickedUpgradeMaxHpBtn();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnClickedUpgradeCurrentHpBtn();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnClickedUpgradeMaxMagazineBtn();
	UFUNCTION(BlueprintCallable, Category = "Code")
		void OnClickedUpgradeDamageBtn();
		
	void SetOwner(APlayerCPP* player) { Owner = player; };
	

	UFUNCTION()
		void SetNeedUpgradeIcon(uint8 inputUpgradeNum, uint8 inputNeeItemNum1, uint8 inputNeeItemAmount1, uint8 inputNeeItemNum2, uint8 inputNeeItemAmount2, bool isMax = false);	//0 : damage  1 : firerate	2 : magazine 3 : fullhp  4 : repair
private:

	UPROPERTY(meta = (BindWidget))
		class UImage* upgrade_NeedItemNumIcon1;
	UPROPERTY(meta = (BindWidget))
		class UImage* upgrade_NeedItemNumIcon2;
	UPROPERTY(meta = (BindWidget))
		class UImage* upgrade_NeedItemNumIcon3;
	UPROPERTY(meta = (BindWidget))
		class UImage* upgrade_NeedItemNumIcon4;
	UPROPERTY(meta = (BindWidget))
		class UImage* upgrade_NeedItemNumIcon5;
	UPROPERTY(meta = (BindWidget))
		class UImage* upgrade_NeedItemNumIcon6;
	UPROPERTY(meta = (BindWidget))
		class UImage* upgrade_NeedItemNumIcon7;
	UPROPERTY(meta = (BindWidget))
		class UImage* upgrade_NeedItemNumIcon8;
	UPROPERTY(meta = (BindWidget))
		class UImage* upgrade_NeedItemNumIcon9;
	UPROPERTY(meta = (BindWidget))
		class UImage* upgrade_NeedItemNumIcon10;
	//==========================================

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgrade_NeedItemAmount1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgrade_NeedItemAmount2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgrade_NeedItemAmount3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgrade_NeedItemAmount4;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgrade_NeedItemAmount5;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgrade_NeedItemAmount6;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgrade_NeedItemAmount7;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgrade_NeedItemAmount8;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgrade_NeedItemAmount9;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgrade_NeedItemAmount10;


	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (BindWidgetOptinal))
		TArray <class UTexture2D*> item_IconArray;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgradeDamage_Max;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgradeFireRate_Max;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgradeMaxMagazine_Max;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* upgradeMaxHp_Max;
};
