// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)
#pragma once

#if WITH_DEV_AUTOMATION_TESTS
#include "CoreMinimal.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/DefaultValueHelper.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

/** Read-only snapshots captured before retiring the legacy whole-vehicle backend.
 * Tests never regenerate expectations from the implementation under test. */
class FVehicleFrozenBaseline
{
public:
    bool Load(FAutomationTestBase& Test, const TCHAR* Name, int32 ExpectedRows, int32 ExpectedColumns)
    {
        const auto Plugin = IPluginManager::Get().FindPlugin(TEXT("KinetiForge"));
        if (!Test.TestTrue(TEXT("Baseline plugin is available"), Plugin.IsValid())) return false;
        const FString Path = FPaths::Combine(Plugin->GetBaseDir(),
            TEXT("Source/KinetiForge/Private/Tests/Baselines/VehicleRuntime-v1"), Name);
        TArray<FString> Lines;
        if (!Test.TestTrue(Path + TEXT(" loads"), FFileHelper::LoadFileToStringArray(Lines, *Path))) return false;
        if (!Test.TestEqual(Path + TEXT(" row count"), Lines.Num(), ExpectedRows)) return false;
        Rows.Reset();
        for (const FString& Line : Lines)
        {
            TArray<FString> Fields;
            Line.ParseIntoArray(Fields, TEXT(","), false);
            if (!Test.TestEqual(Path + TEXT(" column count"), Fields.Num(), ExpectedColumns)) return false;
            TArray<double> Values;
            for (const FString& Field : Fields)
            {
                double Value = 0.0;
                if (!Test.TestTrue(Path + TEXT(" finite numeric value"),
                    FDefaultValueHelper::ParseDouble(Field, Value) && FMath::IsFinite(Value))) return false;
                Values.Add(Value);
            }
            Rows.Add(MoveTemp(Values));
        }
        return true;
    }

    double Get(int32 Row, int32 Column) const { return Rows[Row][Column]; }

    void CheckRow(FAutomationTestBase& Test, int32 Row, std::initializer_list<double> Actual,
        std::initializer_list<double> AbsoluteTolerances) const
    {
        check(Rows.IsValidIndex(Row) && Rows[Row].Num() == int32(Actual.size())
            && Actual.size() == AbsoluteTolerances.size());
        auto Tolerance = AbsoluteTolerances.begin();
        int32 Column = 0;
        for (double Value : Actual)
        {
            Test.TestTrue(FString::Printf(TEXT("Frozen row %d column %d: %.9g vs %.9g"),
                Row, Column, Value, Rows[Row][Column]), FMath::IsFinite(Value)
                && FMath::Abs(Value - Rows[Row][Column]) <= *Tolerance);
            ++Column;
            ++Tolerance;
        }
    }
private:
    TArray<TArray<double>> Rows;
};
#endif
