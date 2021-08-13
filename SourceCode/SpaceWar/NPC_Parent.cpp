// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Parent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "QuestWidget.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "Player_Human.h"
// Sets default values
ANPC_Parent::ANPC_Parent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	RootComponent = collComp;
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	interactionCollComp = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Area"));
	interactionCollComp->SetupAttachment(collComp);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(collComp);
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.f, 0.f));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;//rotate arm based on the controller
	//SpringArm->bEnableCameraLag = true;
	//SpringArm->CameraLagSpeed = 3.0f;

	//Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ANPC_Parent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

/*
// Called every frame
void ANPC_Parent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

void ANPC_Parent::AddQuest(class APlayer_Human* player)
{
	if (isNPCGiveQuest) player->AddRemoveQuest(questNumber, targetEnemyNum1, RemoveGoal1, title);
}

uint8 ANPC_Parent::GetQuestNumber()
{
	return questNumber;
}

void ANPC_Parent::UpdateTalk(class APlayer_Human* player)
{
	
	if (!isNPCGiveQuest)
	{
		player->SettingQuestText(NPCName, title, conversation_BeforeAccept);
		return;
	}

	bool isAlreadyHave = false;
	uint8 count = 0;
	for (uint8 temp : player->GetReceiveQuestNumber())
	{
		if (temp == questNumber)
		{
			isAlreadyHave = true;
			break;
		}
		count++;
	}
	
	if (isAlreadyHave && player->IsClearQuest(questNumber))
	{
		player->SettingQuestText(NPCName, title, conversation_AfterClear, GoalText, FText::AsNumber(RemoveGoal1));
	}
	else if (!isAlreadyHave)
	{
		player->SettingQuestText(NPCName, title, conversation_BeforeAccept, GoalText, FText::AsNumber(RemoveGoal1));
	}
	else
	{
		player->SettingQuestText(NPCName, title, conversation_DuringQuest, GoalText, FText::AsNumber(RemoveGoal1));
	}

	
}

bool ANPC_Parent::IsGiveQuestNPC()
{
	return isNPCGiveQuest;
}