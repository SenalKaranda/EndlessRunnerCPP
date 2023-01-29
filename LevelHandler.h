// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelHandler.generated.h"

class ABaseLevel;

UCLASS()
class SIDERUNNERCPP_API ALevelHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UFUNCTION()
	void SpawnLevel(bool isFirst);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	APawn* Player;

	TArray<ABaseLevel*> LevelList;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseLevel>> Levels;

public:

	int RandomLevelIndex = 0;

	FVector SpawnLocation = FVector();
	FRotator SpawnRotation = FRotator();

	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
};
