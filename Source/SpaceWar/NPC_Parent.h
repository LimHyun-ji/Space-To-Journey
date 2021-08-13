// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC_Parent.generated.h"

UCLASS()
class SPACEWAR_API ANPC_Parent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC_Parent();

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UBoxComponent* collComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class USphereComponent* interactionCollComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* SpringArm;

	//Follow Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* FollowCamera;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Setting", meta = (AllowPrivateAccess))
		uint8 questNumber = 0;

	UPROPERTY(EditAnywhere, Category = "Setting", meta = (AllowPrivateAccess))
		bool isNPCGiveQuest = false;

	UPROPERTY(EditAnywhere, Category = "Setting")
		uint8 targetEnemyNum1 = 0;
	UPROPERTY(EditAnywhere, Category = "Setting")
		uint8 RemoveGoal1 = 0;

	UPROPERTY(EditAnywhere, Category = "Setting")


		FText GoalText = FText::FromString("goal");
	UPROPERTY(EditAnywhere, Category = "Setting")
		FText title = FText::FromString("title");
	UPROPERTY(EditAnywhere, Category = "Setting")
		FText conversation_BeforeAccept = FText::FromString("conversation");
	UPROPERTY(EditAnywhere, Category = "Setting")
		FText conversation_DuringQuest = FText::FromString("conversation");
	UPROPERTY(EditAnywhere, Category = "Setting")
		FText conversation_AfterClear = FText::FromString("conversation");
	UPROPERTY(EditAnywhere, Category = "Setting")
		FText NPCName = FText::FromString("Name");

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void AddQuest(class APlayer_Human* player);

	UFUNCTION()
		void UpdateTalk(class APlayer_Human* player);
	UFUNCTION()
		bool IsGiveQuestNPC();

	UFUNCTION()
		uint8 GetQuestNumber();
};
