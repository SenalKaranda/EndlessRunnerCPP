// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunnerChar.generated.h"

UCLASS()
class SIDERUNNERCPP_API ARunnerChar : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* SideViewCamera;

public:
	// Sets default values for this character's properties
	ARunnerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	void Move(float value);

public:

	class UCameraComponent* GetSideViewCameraComponent() const
	{
		return SideViewCamera;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RestartLevel();
	virtual void RestartLevel_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayerDeath();
	virtual void PlayerDeath_Implementation();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	float zPosition;
	FVector tempPos;
	FVector startPos;

	bool canMove;

	UPROPERTY(BlueprintReadOnly)
	float distanceScore;

	UPROPERTY(BlueprintReadWrite)
	float distanceHighScore;
};
