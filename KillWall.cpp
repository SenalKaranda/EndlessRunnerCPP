// Fill out your copyright notice in the Description page of Project Settings.


#include "KillWall.h"

// Sets default values
AKillWall::AKillWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKillWall::BeginPlay()
{
	Super::BeginPlay();
	
	this->GetRootComponent()->ComponentVelocity = FVector(0, 25, 0);
}

// Called every frame
void AKillWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + FVector(0, Speed * DeltaTime, 0), true);

}

