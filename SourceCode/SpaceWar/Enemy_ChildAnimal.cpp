#include "Enemy_ChildAnimal.h"
#include <Components/BoxComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include "Planet_EnemyParent.h"
#include "Player_human.h"

// Sets default values
AEnemy_ChildAnimal::AEnemy_ChildAnimal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	attackInteraction = CreateDefaultSubobject<UBoxComponent>(TEXT("attackInteraction"));
	attackInteraction->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy_ChildAnimal::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy_ChildAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!target->isDead)
	{
		float distance = GetDistanceTo(target);
		if (!isDead)
		{
			if (distance < 1000)
			{
				AttackMove();
				isTracking = true;
				setRotating = false;
			}
			//일정 범위 내에서 랜덤k하게 움직이기
			else
			{
				isTracking = false;
				if ((InitLocation.X - 1000 <= GetActorLocation().X && GetActorLocation().X <= InitLocation.X + 3000)
					&& (InitLocation.Y - 1000 <= GetActorLocation().Y && GetActorLocation().Y <= InitLocation.Y + 3000))
				{
					RandomMove();
				}
				else
				{	//플레이어와 가깝지 않고 구역을 벗어나면 다시 돌아오기
					ReturnMove();
					setRotating = false;
				}
			}
		}
		else
		{
			attackDamage = 0;
			isAttack = false;
			target->isHit = false;
		}

		if (isAttack)
		{
			currentAttackDelay += GetWorld()->DeltaTimeSeconds;

			if (currentAttackDelay > attackDelay)
			{
				target->playerHP -= attackDamage;
				currentAttackDelay = 0;
			}
		}

	}

	Dead();
}

void AEnemy_ChildAnimal::RandomMove()
{
	//항상 forward로 움직이고 rotate만 바꾼다
	FRotator newRotation;
	if (!setRotating)
	{
		randomGoalRotation.Yaw = FMath::RandRange(-180, 180);
		setRotating = true;
	}

	//1씩 더하며 rotate
	if (randomGoalRotation.Yaw > GetActorRotation().Yaw)
		newRotation.Yaw = GetActorRotation().Yaw + 1;
	else if (randomGoalRotation.Yaw < GetActorRotation().Yaw)
		newRotation.Yaw = GetActorRotation().Yaw - 1;

	if (newRotation.Yaw - 10 <= randomGoalRotation.Yaw && randomGoalRotation.Yaw <= newRotation.Yaw + 10)
	{
		setRotating = false;
	}
	SetActorRotation(newRotation);

	//앞으로 가기 방향
	FVector dir = GetActorForwardVector();
	dir.Normalize();

	FVector P0 = GetActorLocation();
	FVector Vt = dir * speed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + Vt;

	SetActorLocation(P);
}

void AEnemy_ChildAnimal::ReturnMove()
{
	FVector currentLocation = GetActorLocation();
	FVector newLocation;
	FVector dir = InitLocation - currentLocation;
	dir.Normalize();

	FRotator newRotation;
	returnGoalRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, InitLocation);

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
		newLocation= currentLocation + dir * speed * GetWorld()->DeltaTimeSeconds;
		SetActorLocation(newLocation);
	}

}


void AEnemy_ChildAnimal::AttackMove()
{

	if (!isAttack)
	{
		//MoveToTarget
		//어디로 움직일 지 방향을 구한다
		FVector currentLocation = GetActorLocation();
		FVector targetLocation = target->GetActorLocation();
		FVector newLocation;
		FVector dir = targetLocation - currentLocation;
		dir.Normalize();
		//target과 본인의 location 차이로 어느 정도 회전할지 구하기
		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, targetLocation);
		//newRotation.Pitch = 0;
		newRotation.Roll = 0;
		newRotation.Pitch = 0;
		SetActorRotation(newRotation);

		newLocation= currentLocation + dir * speed * 4 * GetWorld()->DeltaTimeSeconds;
		SetActorLocation(newLocation);
	}
}


void AEnemy_ChildAnimal::OnAttackOverlap_Begin(AActor* OtherActor)
{
	if (OtherActor != this)
	{
		if (OtherActor == target && !isDead)
		{
			isAttack = true;
		}
	}
}

void AEnemy_ChildAnimal::OnAttackOverlap_End(AActor* OtherActor)
{
	if (OtherActor != this)
	{
		if (OtherActor == target && !isDead)
		{
			isAttack = false;
		}
	}
}

bool AEnemy_ChildAnimal::GetIsTracking()
{
	return isTracking;
}
