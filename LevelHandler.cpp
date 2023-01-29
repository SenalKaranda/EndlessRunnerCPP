// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelHandler.h"
#include "BaseLevel.h"
#include "Components/BoxComponent.h"

// Sets default values
ALevelHandler::ALevelHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelHandler::BeginPlay()
{
	Super::BeginPlay();

	SpawnLevel(true);
	SpawnLevel(false);
	SpawnLevel(false);
	SpawnLevel(false);
	
}

// Called every frame
void ALevelHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelHandler::SpawnLevel(bool isFirst)
{
	SpawnLocation = FVector(0.f, 1000.f, 0.f);
	SpawnRotation = FRotator(0, 90, 0);

	ABaseLevel* LastLevel = nullptr;
	ABaseLevel* NewLevel = nullptr;

	if (!isFirst)
	{
		LastLevel = LevelList.Last();
		SpawnLocation = LastLevel->GetSpawnLocation()->GetComponentTransform().GetTranslation();
		RandomLevelIndex = FMath::RandRange(0, (Levels.Num() - 1));
	}
	
	NewLevel = GetWorld()->SpawnActor<ABaseLevel>(Levels[RandomLevelIndex], SpawnLocation, SpawnRotation, SpawnInfo);

	if (NewLevel)
	{
		if (NewLevel->GetTrigger())
		{
			NewLevel->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &ALevelHandler::OnOverlapBegin);
		}
	}

	LevelList.Add(NewLevel);
	if (LevelList.Num() > 5)
	{
		LevelList.RemoveAt(0);
	}
}

void ALevelHandler::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SpawnLevel(false);
}

