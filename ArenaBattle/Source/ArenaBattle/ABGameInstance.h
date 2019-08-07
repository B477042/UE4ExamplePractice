// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "ABGameInstance.generated.h"

/**
 * chapter 11
 step 1. 엑셀 데이터를 불러와 데이터 관리
 step 2. 로직 분산 시키기
 step 3. UI만들기
 */
//data load sturct
USTRUCT(BlueprintType)
struct FABCharacterData :public FTableRowBase
{
	GENERATED_BODY()
public:
	FABCharacterData():Level(1),MaxHP(100.0f),DropExp(10),NextExp(30){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 DropExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 NextExp;


};


UCLASS()
class ARENABATTLE_API UABGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UABGameInstance();
	virtual void Init()override;
	FABCharacterData* GetABCharacterData(int32 Level);

private:
	UPROPERTY()
		class UDataTable* ABCharacterTable;
};
