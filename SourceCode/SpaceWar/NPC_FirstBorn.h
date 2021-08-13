// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC_Parent.h"
#include "NPC_FirstBorn.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API ANPC_FirstBorn : public ANPC_Parent
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ANPC_FirstBorn();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



private:
	

	
	
};
