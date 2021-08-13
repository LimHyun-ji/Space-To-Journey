// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Animal.h"
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "SpawnManager.h"
#include "Player_Human.h"
#include "H_PlayerBullet.h"
#include "SkillAttack1.h"

// Sets default values
AEnemy_Animal::AEnemy_Animal()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	collComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Box"));
	RootComponent = collComp;

	attackInteraction = CreateDefaultSubobject<UBoxComponent>(TEXT("attackInteraction"));
	attackInteraction->SetupAttachment(RootComponent);

	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	meshComp->SetupAttachment(collComp);
	
}

// Called when the game starts or when spawned
void AEnemy_Animal::BeginPlay()
{
	Super::BeginPlay();
	//enemy�ʱ� ���� ��ġ
	InitLocation = GetActorLocation();

	//find Target
	TArray <AActor*> obj1;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Human::StaticClass(), obj1);
	for (AActor* actor : obj1)
	{
		target = Cast<APlayer_Human>(actor);
		if (IsValid(target))
			break;
	}
}

// Called every frame
void AEnemy_Animal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//�÷��̾�� ������� Ȯ��
	if (!target->isDead)
	{
		float distance = GetDistanceTo(target);
		if (!isDead)
		{
			if (distance < 1000)
			{
				//rotate�� ���������� ����->����ǥ ���� �� ���� ���� �� ����
				MoveToTarget(DeltaTime);
				isTracking = true;
				setRotating = false;
			}
			//���� ���� ������ ����k�ϰ� �����̱�
			else
			{
				isTracking = false;
				if ((InitLocation.X - 1000 <= GetActorLocation().X && GetActorLocation().X <= InitLocation.X + 3000)
					&& (InitLocation.Y - 1000 <= GetActorLocation().Y && GetActorLocation().Y <= InitLocation.Y + 3000))
				{
					RandomMove();
				}
				else
				{	//�÷��̾�� ������ �ʰ� ������ ����� �ٽ� ���ƿ���
					ReturnMove(DeltaTime);
					setRotating = false;
				}
			}
		}
		else
		{
			attackDamage = 0;
			isAttackActivation = false;
			target->isHit = false;
		}

		if (isAttackActivation)
		{
			currentAttackDelay += GetWorld()->DeltaTimeSeconds;

			if (currentAttackDelay > attackDelay)
			{
				target->playerHP -= attackDamage;
				currentAttackDelay = 0;
			}
		}

	}

	/////////HP�� 0�̶��/////////////
	if (animalHP <= 0)
	{
		if (isDead)
		{
			collComp->SetSimulatePhysics(false);
			collComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			attackInteraction->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		isDead = true;

		deathCountDown -= GetWorld()->DeltaTimeSeconds;
		if (deathCountDown < 0)
		{
			target->SetPlayerMP(target->GetPlayerMP() + 10);
			Destroy();
		}
	}

}

// Called to bind functionality to input
void AEnemy_Animal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


//���� ���� �ȿ��� random�ϰ� move
void AEnemy_Animal::RandomMove()
{
	//�׻� forward�� �����̰� rotate�� �ٲ۴�
	//UE_LOG(LogTemp, Warning, TEXT("RandomMove"));
	FRotator newRotation;
	if (!setRotating)
	{
		randomGoalRotation.Yaw = FMath::RandRange(-180, 180);
		setRotating = true;
	}

	//1�� ���ϸ� rotate
	if (randomGoalRotation.Yaw > GetActorRotation().Yaw)
		newRotation.Yaw = GetActorRotation().Yaw + 1;
	else if (randomGoalRotation.Yaw < GetActorRotation().Yaw)
		newRotation.Yaw = GetActorRotation().Yaw - 1;

	if (newRotation.Yaw - 10 <= randomGoalRotation.Yaw && randomGoalRotation.Yaw <= newRotation.Yaw + 10)
	{
		setRotating = false;
	}
	SetActorRotation(newRotation);
	//UE_LOG(LogTemp, Warning, TEXT("RandomMove Yaw %f"), randomGoalRotation.Yaw);

	//������ ���� ����
	FVector dir = GetActorForwardVector();
	dir.Normalize();

	FVector P0 = GetActorLocation();
	FVector Vt = dir * speed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + Vt;

	SetActorLocation(P);
}


void AEnemy_Animal::ReturnMove(float deltaTime)
{
	
	//�ʱ� ��ġ�� ���ư���
	//UE_LOG(LogTemp, Warning, TEXT("Return Move"));
	
	FVector currentLocation = GetActorLocation();
	FVector dir = InitLocation - currentLocation;
	dir.Normalize();

	FRotator newRotation;
	returnGoalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, InitLocation);

	/*
	if (returnGoalRotation.Yaw > GetActorRotation().Yaw)
		newRotation.Yaw = GetActorRotation().Yaw + 1;
	else if (returnGoalRotation.Yaw < GetActorRotation().Yaw)
		newRotation.Yaw = GetActorRotation().Yaw - 1;
	*/

	if (abs(GetActorRotation().Yaw - returnGoalRotation.Yaw) < 0.7)
	{
		newRotation.Yaw = GetActorRotation().Yaw;
	}
	else
	{
		newRotation.Yaw = GetActorRotation().Yaw + (-1 * (abs(GetActorRotation().Yaw - returnGoalRotation.Yaw) - 180)) / abs((abs(GetActorRotation().Yaw - returnGoalRotation.Yaw) - 180)) * (abs(returnGoalRotation.Yaw - GetActorRotation().Yaw) / (returnGoalRotation.Yaw - GetActorRotation().Yaw)) * 1;
	}

	newRotation.Pitch = 0;
	newRotation.Roll = 0;

	SetActorRotation(newRotation);
	if (newRotation.Yaw - 10 <= returnGoalRotation.Yaw && returnGoalRotation.Yaw <= newRotation.Yaw + 10)
	{
		SetActorLocation(currentLocation + dir * speed * deltaTime);
	}


	/*
	FVector currentLocation = GetActorLocation();
	
	FRotator goalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, InitLocation);
	FRotator currentRotation = GetActorRotation();
	FRotator newRotation;
	if (abs(currentRotation.Yaw - goalRotation.Yaw) < 0.7)
	{
		newRotation.Yaw = currentRotation.Yaw;
	}
	else
	{
		newRotation.Yaw = currentRotation.Yaw + (-1 * (abs(currentRotation.Yaw - goalRotation.Yaw) - 180)) / abs((abs(currentRotation.Yaw - goalRotation.Yaw) - 180)) * (abs(goalRotation.Yaw - currentRotation.Yaw) / (goalRotation.Yaw - currentRotation.Yaw)) * 1;
	}
	
	SetActorRotation(FRotator(0, newRotation.Yaw, 0));

	
	if (newRotation.Yaw - 10 <= returnGoalRotation.Yaw && returnGoalRotation.Yaw <= newRotation.Yaw + 10)
	{
		SetActorLocation(currentLocation + GetActorForwardVector() * speed * deltaTime);		
	}
	*/
}


void AEnemy_Animal::MoveToTarget(float deltaTime)
{
	if (!isAttackActivation)
	{
		//���� ������ �� ������ ���Ѵ�
		FVector currentLocation = GetActorLocation();
		FVector targetLocation = target->GetActorLocation();
		FVector dir = targetLocation - currentLocation;
		dir.Normalize();
		//target�� ������ location ���̷� ��� ���� ȸ������ ���ϱ�
		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);
		//newRotation.Pitch = 0;
		newRotation.Roll = 0;
		newRotation.Pitch = 0;
		SetActorRotation(newRotation);

		SetActorLocation(currentLocation + dir * speed * 4 * deltaTime);
		

	}
}

void AEnemy_Animal::OnAttackOverlap_Begin(AActor* OtherActor)
{
	if (OtherActor != this)
	{
		if (OtherActor == target && !isDead)
		{
			isAttackActivation = true;
		}
	}
}

void AEnemy_Animal::OnAttackOverlap_End(AActor* OtherActor)
{
	if (OtherActor != this)
	{
		if (OtherActor == target && !isDead)
		{
			isAttackActivation = false;
		}
	}
}

void AEnemy_Animal::OnOverlapBegin(AActor* OtherActor)
{
	playerBullet = Cast<AH_PlayerBullet>(OtherActor);
	skillAttack1 = Cast<ASkillAttack1>(OtherActor);

	if (OtherActor != this)
	{
		if (OtherActor == playerBullet)
		{
			isHit = true;
			animalHP -= playerBullet->GetBulletDamage();
			playerBullet->ExploreEffect();
		}
		else if (OtherActor == skillAttack1)
		{
			isHit = true;
			animalHP -= skillAttack1->GetAttackDamage();
		}
	}

}
void AEnemy_Animal::OnOverlapEnd(AActor* OtherActor)
{
	playerBullet = Cast<AH_PlayerBullet>(OtherActor);
	skillAttack1 = Cast<ASkillAttack1>(OtherActor);

	if (OtherActor != this)
	{
		if (OtherActor == playerBullet || skillAttack1)
		{
			isHit = false;
		}
	}

}

/////////Getter Setter/////////////////

bool AEnemy_Animal::GetIsAttackActivation()
{
	return isAttackActivation;
}
void AEnemy_Animal::SetIsAttackActivation()
{
	isAttackActivation = false;
}
bool AEnemy_Animal::GetIsHit()
{
	return isHit;
}

void AEnemy_Animal::SetIsHit()
{
	isHit = false;
}

int AEnemy_Animal::GetAttackDamage()
{
	return attackDamage;
}

bool AEnemy_Animal::GetIsDead()
{
	return isDead;
}

bool AEnemy_Animal::GetIsTracking()
{
	return isTracking;
}
