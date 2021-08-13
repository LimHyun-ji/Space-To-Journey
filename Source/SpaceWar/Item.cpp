// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Comp"));
	RootComponent = collComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractionArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Area"));
	InteractionArea->SetupAttachment(collComp);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshRefstandard(TEXT("StaticMesh'/Game/StarterContent/Props/SM_CornerFrame.SM_CornerFrame'"));

	if (meshRefstandard.Succeeded())
	{
		meshComp->SetStaticMesh(meshRefstandard.Object);
	}

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	ItemMesh();

	if ((0 <= itemNumber) && (itemNumber <= 99))
	{
		itemType = 0;
	}
	else if ((100 <= itemNumber) && (itemNumber <= 199))
	{
		itemType = 1;
	}

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	
}

void AItem::ItemMesh()
{
	
	meshComp->SetStaticMesh(ItemData[itemNumber].mesh);
	meshComp->SetRelativeScale3D(ItemData[itemNumber].meshScale);
	InteractionArea->SetWorldScale3D(ItemData[itemNumber].meshInteractionScale);
	collComp->SetEnableGravity(ItemData[itemNumber].isMeshGravity);
	collComp->SetBoxExtent(ItemData[itemNumber].meshBlockScale);

}

void AItem::SetItemNumber(uint8 inputItemNumber)
{
	itemNumber = inputItemNumber;
	ItemMesh();
}

uint8 AItem::GetItemNumber()
{
	return itemNumber;
}

UTexture2D* AItem::GetItemIcon(uint8 InputItemNumber)
{
	return ItemData[InputItemNumber].Item_IconArray;
}

TArray <class UTexture2D*> AItem::GetItemAllIcon()
{
	TArray <UTexture2D*> temp;
	uint8 count = 0;
	
	for (FItemDataStruct Icon : ItemData)
	{		
		temp.Add(ItemData[count].Item_IconArray);

		count++;
	}

	return temp;
}


/*

AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Comp"));
	RootComponent = collComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractionArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Area"));
	InteractionArea->SetupAttachment(collComp);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshRefstandard(TEXT("StaticMesh'/Game/StarterContent/Props/SM_CornerFrame.SM_CornerFrame'"));

	if (meshRefstandard.Succeeded())
	{
		meshComp->SetStaticMesh(meshRefstandard.Object);
	}

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	ItemMesh();

	if ((0 <= itemNumber) && (itemNumber <= 99))
	{
		itemType = 0;
	}
	else if ((100 <= itemNumber) && (itemNumber <= 199))
	{
		itemType = 1;
	}

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

void AItem::ItemMesh()
{

	meshComp->SetStaticMesh(mesh[itemNumber]);
	meshComp->SetRelativeScale3D(meshScale[itemNumber]);
	InteractionArea->SetWorldScale3D(meshInteractionScale[itemNumber]);
	collComp->SetEnableGravity(isMeshGravity[itemNumber]);
	collComp->SetBoxExtent(meshBlockScale[itemNumber]);

}

void AItem::SetItemNumber(uint8 inputItemNumber)
{
	itemNumber = inputItemNumber;
	ItemMesh();
}

uint8 AItem::GetItemNumber()
{
	return itemNumber;
}

UTexture2D* AItem::GetItemIcon(uint8 InputItemNumber)
{
	return Item_IconArray[InputItemNumber];
}

TArray <class UTexture2D*> AItem::GetItemAllIcon()
{
	return Item_IconArray;
}

*/