// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PatrolPath.h"

// Sets default values
ACPP_PatrolPath::ACPP_PatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_PatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_PatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

