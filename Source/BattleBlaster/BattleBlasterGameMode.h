// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tank.h"
#include "ScreenMessage.h"
#include "BattleBlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenMessage> ScreenMessageClass;

	UPROPERTY(EditAnywhere)
	float GameOverDelay = 3.0f;

	UPROPERTY(EditAnywhere)
	int32 CountDownDelay = 3;

	int32 CurrentCountDown;
	
	FTimerHandle CountDownTimerHandle;

	ATank* Tank;
	int32 TowerCount;
	bool IsVictory = false;
	UScreenMessage* ScreenMessageWidget;

	void ActorDied(AActor* DeadActor);
	void GameOverTimeout();
	void CountDownTimer();
};
