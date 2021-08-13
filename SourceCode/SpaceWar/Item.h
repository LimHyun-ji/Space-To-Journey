// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

USTRUCT(Atomic)
struct FItemDataStruct
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditDefaultsOnly)
		class UStaticMesh* mesh;

	UPROPERTY(EditDefaultsOnly)
		FVector meshScale;
	UPROPERTY(EditDefaultsOnly)
		bool isMeshGravity;
	UPROPERTY(EditDefaultsOnly)
		FVector meshInteractionScale;
	UPROPERTY(EditDefaultsOnly)
		FVector meshBlockScale;


	UPROPERTY(EditDefaultsOnly, Category = "SettingIcon")
		class UTexture2D* Item_IconArray;
};


UCLASS()
class SPACEWAR_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* collComp;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* InteractionArea;
	
	

	UPROPERTY(EditDefaultsOnly, Category = "SettingItemData")
		TArray <FItemDataStruct> ItemData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void ItemMesh();
	UFUNCTION()
		void SetItemNumber(uint8 inputItemNumber);
	UFUNCTION()
		uint8 GetItemNumber();
	UFUNCTION()
		UTexture2D* GetItemIcon(uint8 InputItemNumber);
	UFUNCTION()
		TArray <class UTexture2D*> GetItemAllIcon();

private:
	UPROPERTY(EditAnywhere, Category = "Setting", meta = (AllowPrivateAccess = true))
		uint8 itemNumber = 0;			//0~99 : 아이템 타입 0		100~199 : 아이템 타입 1
	UPROPERTY()
		uint8 itemType = 0;		//0: 기타		1: 소비




};

/*


UCLASS()
class SPACEWAR_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* collComp;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* InteractionArea;

	UPROPERTY(EditDefaultsOnly, Category = "SettingMesh")
		TArray <class UStaticMesh*> mesh;

	UPROPERTY(EditDefaultsOnly, Category = "SettingMeshScale")
		TArray <FVector> meshScale;
	UPROPERTY(EditDefaultsOnly, Category = "SettingGravity")
		TArray <bool> isMeshGravity;
	UPROPERTY(EditDefaultsOnly, Category = "SettingInteractionScale")
		TArray < FVector> meshInteractionScale;
	UPROPERTY(EditDefaultsOnly, Category = "SettingBlockScale")
		TArray < FVector> meshBlockScale;


	UPROPERTY(EditDefaultsOnly, Category = "SettingIcon")
		TArray <class UTexture2D*> Item_IconArray;
	UPROPERTY(EditDefaultsOnly, Category = "test")
		TArray <FItemStruct> test;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void ItemMesh();
	UFUNCTION()
		void SetItemNumber(uint8 inputItemNumber);
	UFUNCTION()
		uint8 GetItemNumber();
	UFUNCTION()
		UTexture2D* GetItemIcon(uint8 InputItemNumber);
	UFUNCTION()
		TArray <class UTexture2D*> GetItemAllIcon();

private:
	UPROPERTY(EditAnywhere, Category = "Setting", meta = (AllowPrivateAccess = true))
		uint8 itemNumber = 0;			//0~99 : 아이템 타입 0		100~199 : 아이템 타입 1
	UPROPERTY()
		uint8 itemType = 0;		//0: 기타		1: 소비




};
*/

