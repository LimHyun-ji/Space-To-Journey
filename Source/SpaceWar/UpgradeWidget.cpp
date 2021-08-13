// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include <Components/TextBlock.h>
#include "Engine/Texture2D.h"

bool UUpgradeWidget::Initialize()
{
	Super::Initialize();

	return true;
}

void UUpgradeWidget::OnClickedUpgradeFireRateBtn()
{
	Owner->UpgradeFireRate();
}

void UUpgradeWidget::OnClickedUpgradeMaxHpBtn()
{
	Owner->UpgradeMaxHp();
}

void UUpgradeWidget::OnClickedUpgradeCurrentHpBtn()
{
	Owner->UpgradeCurrentHp();
}

void UUpgradeWidget::OnClickedUpgradeMaxMagazineBtn()
{
	Owner->UpgradeMaxMagazine();
}

void UUpgradeWidget::OnClickedUpgradeDamageBtn()
{
	Owner->UpgradeDamage();
}

void UUpgradeWidget::SetNeedUpgradeIcon(uint8 inputUpgradeNum, uint8 inputNeeItemNum1, uint8 inputNeeItemAmount1, uint8 inputNeeItemNum2, uint8 inputNeeItemAmount2, bool isMax)
{
	switch (inputUpgradeNum)
	{
	case 0:
		if (isMax)
		{
			upgradeDamageBtn->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemNumIcon1->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemAmount1->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemNumIcon2->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemAmount2->SetVisibility(ESlateVisibility::Hidden);
			upgradeDamage_Max->SetText(FText::FromString("Max"));
			break;
		}
		upgrade_NeedItemNumIcon1->SetBrushFromTexture(item_IconArray[inputNeeItemNum1]);
		upgrade_NeedItemAmount1->SetText(FText::AsNumber(inputNeeItemAmount1));
		upgrade_NeedItemNumIcon2->SetBrushFromTexture(item_IconArray[inputNeeItemNum2]);
		upgrade_NeedItemAmount2->SetText(FText::AsNumber(inputNeeItemAmount2));		
		break;
	case 1:
		if (isMax)
		{
			upgradeFireRateBtn->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemNumIcon3->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemAmount3->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemNumIcon4->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemAmount4->SetVisibility(ESlateVisibility::Hidden);
			upgradeFireRate_Max->SetText(FText::FromString("Max"));
			break;
		}
		upgrade_NeedItemNumIcon3->SetBrushFromTexture(item_IconArray[inputNeeItemNum1]);
		upgrade_NeedItemAmount3->SetText(FText::AsNumber(inputNeeItemAmount1));
		upgrade_NeedItemNumIcon4->SetBrushFromTexture(item_IconArray[inputNeeItemNum2]);
		upgrade_NeedItemAmount4->SetText(FText::AsNumber(inputNeeItemAmount2));
		break;
	case 2:
		if (isMax)
		{
			upgradeMaxMagazineBtn->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemNumIcon5->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemAmount5->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemNumIcon6->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemAmount6->SetVisibility(ESlateVisibility::Hidden);
			upgradeMaxMagazine_Max->SetText(FText::FromString("Max"));
			break;
		}
		upgrade_NeedItemNumIcon5->SetBrushFromTexture(item_IconArray[inputNeeItemNum1]);
		upgrade_NeedItemAmount5->SetText(FText::AsNumber(inputNeeItemAmount1));
		upgrade_NeedItemNumIcon6->SetBrushFromTexture(item_IconArray[inputNeeItemNum2]);
		upgrade_NeedItemAmount6->SetText(FText::AsNumber(inputNeeItemAmount2));
		break;
	case 3:
		if (isMax)
		{
			upgradeMaxHpBtn->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemNumIcon7->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemAmount7->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemNumIcon8->SetVisibility(ESlateVisibility::Hidden);
			upgrade_NeedItemAmount8->SetVisibility(ESlateVisibility::Hidden);
			upgradeMaxHp_Max->SetText(FText::FromString("Max"));
			break;
		}
		upgrade_NeedItemNumIcon7->SetBrushFromTexture(item_IconArray[inputNeeItemNum1]);
		upgrade_NeedItemAmount7->SetText(FText::AsNumber(inputNeeItemAmount1));
		upgrade_NeedItemNumIcon8->SetBrushFromTexture(item_IconArray[inputNeeItemNum2]);
		upgrade_NeedItemAmount8->SetText(FText::AsNumber(inputNeeItemAmount2));
		break;
	case 4:
		upgrade_NeedItemNumIcon9->SetBrushFromTexture(item_IconArray[inputNeeItemNum1]);
		upgrade_NeedItemAmount9->SetText(FText::AsNumber(inputNeeItemAmount1));
		upgrade_NeedItemNumIcon10->SetBrushFromTexture(item_IconArray[inputNeeItemNum2]);
		upgrade_NeedItemAmount10->SetText(FText::AsNumber(inputNeeItemAmount2));


		break;
	}
}