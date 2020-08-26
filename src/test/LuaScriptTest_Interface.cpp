//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "LuaScriptTest_Base.h"

#include "ramses-logic/Property.h"


namespace rlogic
{
    class ALuaScript_Interface : public ALuaScript
    {
    protected:
        void TearDown() override
        {
            std::remove("script.lua");
        }
    };

    // Not testable, because assignment to userdata can't be catched. It's just a replacement of the current value
    TEST_F(ALuaScript_Interface, DISABLED_GeneratesErrorWhenOverwritingInputsInInterfaceFunction)
    {
        auto* script = m_logicEngine.createLuaScriptFromSource(R"(
            function interface()
                IN = {}
            end

            function run()
            end
        )");

        ASSERT_EQ(nullptr, script);

        EXPECT_EQ(m_logicEngine.getErrors().size(), 1u);
        EXPECT_EQ(m_logicEngine.getErrors()[0], "Special global symbol 'IN' should not be overwritten with other types in interface() function!!");
    }

    TEST_F(ALuaScript_Interface, ProducesErrorsIfARuntimeErrorOccursInInterface)
    {
        auto script = m_logicEngine.createLuaScriptFromSource(R"(
            function mul(a,b)
                return a*b
            end

            function interface()
                mul(42)
            end

            function run()
            end
        )");

        ASSERT_EQ(nullptr, script);
        EXPECT_EQ(m_logicEngine.getErrors().size(), 1u);
        EXPECT_EQ(m_logicEngine.getErrors()[0],
                  "[string \"unknown\"]:3: attempt to perform arithmetic on local 'b' (a nil value)\nstack traceback:\n\t[string \"unknown\"]:3: in function 'mul'\n\t[string "
                  "\"unknown\"]:7: in function <[string \"unknown\"]:6>");
    }

    TEST_F(ALuaScript_Interface, ProducesErrorIfTryingToAccessUnexistingPropertyInInterface)
    {
        auto* script = m_logicEngine.createLuaScriptFromSource(R"(
            function interface()
                local this_will_fail = IN.this_does_not_exist
            end

            function run()
            end
        )");

        EXPECT_EQ(nullptr, script);

        ASSERT_EQ(1u, m_logicEngine.getErrors().size());
        EXPECT_THAT(m_logicEngine.getErrors()[0], ::testing::HasSubstr("Trying to access not available property this_does_not_exist in interface!"));
    }

    TEST_F(ALuaScript_Interface, ProducesErrorWhenTryingToCreateInterfacePropertiesWithNonStringIndexAtInterfaceTime)
    {
        const std::vector<std::string> wrongIndexTypes = {"[1]", "[true]", "[{x=5}]", "[nil]"};

        const std::string expectedErrorMessage = "Only strings supported as table key type!";

        std::vector<LuaTestError> allErrorCases;
        for (const auto& errorType : wrongIndexTypes)
        {
            allErrorCases.emplace_back(LuaTestError{"IN" + errorType + " = INT", expectedErrorMessage});
            allErrorCases.emplace_back(LuaTestError{"OUT" + errorType + " = INT", expectedErrorMessage});
        }

        for (const auto& singleCase : allErrorCases)
        {
            auto script = m_logicEngine.createLuaScriptFromSource("function interface()\n" + singleCase.errorCode +
                                                                  "\n"
                                                                  "end\n"
                                                                  "function run()\n"
                                                                  "end\n");

            EXPECT_EQ(nullptr, script);

            ASSERT_EQ(1u, m_logicEngine.getErrors().size());
            EXPECT_THAT(m_logicEngine.getErrors()[0], ::testing::HasSubstr(singleCase.expectedErrorMessage));
        }
    }

    TEST_F(ALuaScript_Interface, ProducesErrorWhenTryingToAccessInterfacePropertiesWithNonStringIndexAtInterfaceTime)
    {
        const std::vector<std::string> wrongIndexTypes = {"[1]", "[true]", "[{x=5}]", "[nil]"};

        const std::string expectedErrorMessage = "Only strings supported as table key type!";

        std::vector<LuaTestError> allErrorCases;
        for (const auto& errorType : wrongIndexTypes)
        {
            allErrorCases.emplace_back(LuaTestError{"IN = IN" + errorType, expectedErrorMessage});
            allErrorCases.emplace_back(LuaTestError{"OUT = OUT" + errorType, expectedErrorMessage});
        }

        for (const auto& singleCase : allErrorCases)
        {
            auto script = m_logicEngine.createLuaScriptFromSource("function interface()\n" + singleCase.errorCode +
                                                                  "\n"
                                                                  "end\n"
                                                                  "function run()\n"
                                                                  "end\n");

            EXPECT_EQ(nullptr, script);

            ASSERT_EQ(1u, m_logicEngine.getErrors().size());
            EXPECT_THAT(m_logicEngine.getErrors()[0], ::testing::HasSubstr(singleCase.expectedErrorMessage));
        }
    }

    TEST_F(ALuaScript_Interface, ProducesErrorWhenTryingToCreateFieldsWithInvalidTypeAtInterfaceTime)
    {
        const std::vector<LuaTestError> allCases = {
            {"IN.not_a_type_0 = nil",
            "Field 'not_a_type_0' has invalid type! Only primitive types, arrays and nested tables obeying the same rules are supported!"},
            {"IN.not_a_type_1 = 'not a type'",
            "Field 'not_a_type_1' has invalid type! Only primitive types, arrays and nested tables obeying the same rules are supported!"},
            {"IN.not_a_type_2 = true",
            "Field 'not_a_type_2' has invalid type! Only primitive types, arrays and nested tables obeying the same rules are supported!"},
            {"IN.not_a_type_3 = {nested_input_problem = 15000}",
            "Field 'nested_input_problem' has invalid type! Only primitive types, arrays and nested tables obeying the same rules are supported!"},
            {"IN.not_a_type_4 = 150000",
            "Field 'not_a_type_4' has invalid type! Only primitive types, arrays and nested tables obeying the same rules are supported!"},
            {"OUT.not_a_type_1 = 'not a type'",
            "Field 'not_a_type_1' has invalid type! Only primitive types, arrays and nested tables obeying the same rules are supported!"},
            {"OUT.not_a_type_2 = true",
            "Field 'not_a_type_2' has invalid type! Only primitive types, arrays and nested tables obeying the same rules are supported!"},
            {"OUT.not_a_type_3 = {nested_output_problem = \"this_should_not_be_string\"}",
            "Field 'nested_output_problem' has invalid type! Only primitive types, arrays and nested tables obeying the same rules are supported!"},
            {"OUT.not_a_type_4 = 150000",
            "Field 'not_a_type_4' has invalid type! Only primitive types, arrays and nested tables obeying the same rules are supported!"},
            {"OUT.no_nested_name = { INT }",
            "Only strings supported as table key type!"},
            {"OUT.no_nested_name2 = { 5 }",
            "Only strings supported as table key type!"},
            {"OUT.no_nested_type = { correct_key = 'but wrong type' }",
            "Field 'correct_key' has invalid type! Only primitive types, arrays and nested tables obeying the same rules are supported!"},
            // TODO re-enable with arrays
            //{"IN.array1 = ARRAY()"                                                          , "error message"},
            //{"IN.array2 = ARRAY('not a type')"                                              , "error message"},
            //{"IN.array3 = ARRAY(INT, 'not a number')"                                       , "error message"},
            //{"IN.array4 = ARRAY(INT, -5)"                                                   , "error message"},
            //{"IN.array5 = ARRAY({}, 5)"                                                     , "error message"},
            //{"IN.array6 = ARRAY({x = INT, forbidden_nested = {nested = INT}}, 5)"           , "error message"},
            //{"IN.array7 = ARRAY(ARRAY(INT, 52), 5)"                                         , "error message"},
        };

        for (const auto& singleCase : allCases)
        {
            auto script = m_logicEngine.createLuaScriptFromSource(
                "function interface()\n" +
                singleCase.errorCode + "\n"
                "end\n"
                "function run()\n"
                "end\n"
            );

            EXPECT_EQ(nullptr, script);
            ASSERT_EQ(1u, m_logicEngine.getErrors().size());
            EXPECT_THAT(m_logicEngine.getErrors()[0], ::testing::HasSubstr(singleCase.expectedErrorMessage));
        }
    }

    TEST_F(ALuaScript_Interface, ProducesErrorIfSamePropertyIsDefinedTwice)
    {
        const std::vector<std::string> declarations =
        {
            "IN.prop = INT",
            "IN.prop = FLOAT",
            "IN.prop = BOOL",
            "IN.prop = STRING",
            "IN.prop = {field = INT}",
            "IN.prop = {}",
            "OUT.prop = INT",
            "OUT.prop = FLOAT",
            "OUT.prop = BOOL",
            "OUT.prop = STRING",
            "OUT.prop = {field = INT}",
            "OUT.prop = {}",
        };

        for(const auto& declaration : declarations)
        {
            auto scriptSource = std::string("function interface()\n");
            scriptSource += declaration;
            scriptSource += "\n";
            scriptSource += declaration;
            scriptSource += "\n"
                "end\n"
                "function run()\n"
                "end\n";

            auto script = m_logicEngine.createLuaScriptFromSource(scriptSource);

            EXPECT_EQ(nullptr, script);

            ASSERT_EQ(1u, m_logicEngine.getErrors().size());
            EXPECT_THAT(m_logicEngine.getErrors()[0], ::testing::HasSubstr("Property 'prop' already exists! Can't declare the same property twice!"));
        }
    }

    TEST_F(ALuaScript_Interface, ReturnsItsTopLevelOutputsByIndex_IndexEqualsOrderOfDeclaration)
    {
        auto* script = m_logicEngine.createLuaScriptFromSource(m_minimalScriptWithOutputs);

        auto outputs = script->getOutputs();

        ASSERT_EQ(10u, outputs->getChildCount());
        EXPECT_EQ("speed", outputs->getChild(0)->getName());
        EXPECT_EQ(EPropertyType::Int32, outputs->getChild(0)->getType());
        EXPECT_EQ("temp", outputs->getChild(1)->getName());
        EXPECT_EQ(EPropertyType::Float, outputs->getChild(1)->getType());
        EXPECT_EQ("name", outputs->getChild(2)->getName());
        EXPECT_EQ(EPropertyType::String, outputs->getChild(2)->getType());
        EXPECT_EQ("enabled", outputs->getChild(3)->getName());
        EXPECT_EQ(EPropertyType::Bool, outputs->getChild(3)->getType());

        // Vec2/3/4 f/i
        EXPECT_EQ("vec2f", outputs->getChild(4)->getName());
        EXPECT_EQ(EPropertyType::Vec2f, outputs->getChild(4)->getType());
        EXPECT_EQ("vec3f", outputs->getChild(5)->getName());
        EXPECT_EQ(EPropertyType::Vec3f, outputs->getChild(5)->getType());
        EXPECT_EQ("vec4f", outputs->getChild(6)->getName());
        EXPECT_EQ(EPropertyType::Vec4f, outputs->getChild(6)->getType());
        EXPECT_EQ("vec2i", outputs->getChild(7)->getName());
        EXPECT_EQ(EPropertyType::Vec2i, outputs->getChild(7)->getType());
        EXPECT_EQ("vec3i", outputs->getChild(8)->getName());
        EXPECT_EQ(EPropertyType::Vec3i, outputs->getChild(8)->getType());
        EXPECT_EQ("vec4i", outputs->getChild(9)->getName());
        EXPECT_EQ(EPropertyType::Vec4i, outputs->getChild(9)->getType());
    }

    TEST_F(ALuaScript_Interface, ReturnsNestedOutputsByIndex_IndexEqualsOrderOfDeclaration_ifDeclaredOneByOne)
    {
        auto* script = m_logicEngine.createLuaScriptFromSource(R"(
            function interface()
                OUT.struct = {}
                OUT.struct.field1 = INT
                OUT.struct.field2 = FLOAT
                OUT.struct.field3 = {}
                OUT.struct.field3.subfield1 = INT
                OUT.struct.field3.subfield2 = FLOAT
            end

            function run()
            end
        )");

        ASSERT_NE(nullptr, script);

        auto outputs = script->getOutputs();
        ASSERT_EQ(1u, outputs->getChildCount());
        auto structField = outputs->getChild(0);
        EXPECT_EQ("struct", structField->getName());
        EXPECT_EQ(EPropertyType::Struct, structField->getType());

        ASSERT_EQ(3u, structField->getChildCount());
        auto field1 = structField->getChild(0);
        auto field2 = structField->getChild(1);
        auto field3 = structField->getChild(2);

        EXPECT_EQ("field1", field1->getName());
        EXPECT_EQ(EPropertyType::Int32, field1->getType());
        EXPECT_EQ("field2", field2->getName());
        EXPECT_EQ(EPropertyType::Float, field2->getType());
        EXPECT_EQ("field3", field3->getName());
        EXPECT_EQ(EPropertyType::Struct, field3->getType());

        ASSERT_EQ(2u, field3->getChildCount());
        auto subfield1 = field3->getChild(0);
        auto subfield2 = field3->getChild(1);

        EXPECT_EQ("subfield1", subfield1->getName());
        EXPECT_EQ(EPropertyType::Int32, subfield1->getType());
        EXPECT_EQ("subfield2", subfield2->getName());
        EXPECT_EQ(EPropertyType::Float, subfield2->getType());
    }

    TEST_F(ALuaScript_Interface, CanDeclarePropertiesProgramatically)
    {
        auto* script = m_logicEngine.createLuaScriptFromSource(R"(
            function interface()
                OUT.root = {}
                local lastStruct = OUT.root
                for i=1,2 do
                    lastStruct["sub" .. tostring(i)] = {}
                    lastStruct = lastStruct["sub" .. tostring(i)]
                end
            end

            function run()
            end
        )");

        ASSERT_NE(nullptr, script);

        auto outputs = script->getOutputs();

        ASSERT_EQ(1u, outputs->getChildCount());
        auto root = outputs->getChild(0);
        EXPECT_EQ("root", root->getName());
        EXPECT_EQ(EPropertyType::Struct, root->getType());

        ASSERT_EQ(1u, root->getChildCount());
        auto sub1 = root->getChild(0);

        EXPECT_EQ("sub1", sub1->getName());
        EXPECT_EQ(EPropertyType::Struct, sub1->getType());

        ASSERT_EQ(1u, sub1->getChildCount());
        auto sub2 = sub1->getChild(0);
        EXPECT_EQ("sub2", sub2->getName());
        EXPECT_EQ(EPropertyType::Struct, sub2->getType());

        EXPECT_EQ(0u, sub2->getChildCount());
    }

    TEST_F(ALuaScript_Interface, AssignsDefaultValuesToItsInputs)
    {
        auto* script = m_logicEngine.createLuaScriptFromSource(m_minimalScriptWithInputs);
        auto  inputs = script->getInputs();

        auto speedInt32 = inputs->getChild("speed");
        auto tempFloat = inputs->getChild("temp");
        auto nameString = inputs->getChild("name");
        auto enabledBool = inputs->getChild("enabled");
        auto vec_2f = inputs->getChild("vec2f");
        auto vec_3f = inputs->getChild("vec3f");
        auto vec_4f = inputs->getChild("vec4f");
        auto vec_2i = inputs->getChild("vec2i");
        auto vec_3i = inputs->getChild("vec3i");
        auto vec_4i = inputs->getChild("vec4i");
        EXPECT_TRUE(speedInt32->get<int32_t>().has_value());
        EXPECT_EQ(0, speedInt32->get<int32_t>().value());
        EXPECT_TRUE(tempFloat->get<float>().has_value());
        EXPECT_EQ(0.0f, tempFloat->get<float>().value());
        EXPECT_TRUE(nameString->get<std::string>().has_value());
        EXPECT_EQ("", nameString->get<std::string>().value());
        EXPECT_TRUE(enabledBool->get<bool>().has_value());
        EXPECT_EQ(false, enabledBool->get<bool>().value());
        EXPECT_TRUE(vec_2f->get<vec2f>());
        EXPECT_TRUE(vec_3f->get<vec3f>());
        EXPECT_TRUE(vec_4f->get<vec4f>());
        EXPECT_TRUE(vec_2i->get<vec2i>());
        EXPECT_TRUE(vec_3i->get<vec3i>());
        EXPECT_TRUE(vec_4i->get<vec4i>());
        vec2f zeroVec2f{ 0.0f, 0.0f };
        vec3f zeroVec3f{ 0.0f, 0.0f, 0.0f };
        vec4f zeroVec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
        vec2i zeroVec2i{ 0, 0 };
        vec3i zeroVec3i{ 0, 0, 0 };
        vec4i zeroVec4i{ 0, 0, 0, 0 };
        EXPECT_EQ(zeroVec2f, *vec_2f->get<vec2f>());
        EXPECT_EQ(zeroVec3f, *vec_3f->get<vec3f>());
        EXPECT_EQ(zeroVec4f, *vec_4f->get<vec4f>());
        EXPECT_EQ(zeroVec2i, *vec_2i->get<vec2i>());
        EXPECT_EQ(zeroVec3i, *vec_3i->get<vec3i>());
        EXPECT_EQ(zeroVec4i, *vec_4i->get<vec4i>());
    }

    TEST_F(ALuaScript_Interface, AssignsDefaultValuesToItsOutputs)
    {
        auto* script = m_logicEngine.createLuaScriptFromSource(m_minimalScriptWithOutputs);
        auto  outputs = script->getOutputs();

        auto speedInt32 = outputs->getChild("speed");
        auto tempFloat = outputs->getChild("temp");
        auto nameString = outputs->getChild("name");
        auto enabledBool = outputs->getChild("enabled");
        auto vec_2f = outputs->getChild("vec2f");
        auto vec_3f = outputs->getChild("vec3f");
        auto vec_4f = outputs->getChild("vec4f");
        auto vec_2i = outputs->getChild("vec2i");
        auto vec_3i = outputs->getChild("vec3i");
        auto vec_4i = outputs->getChild("vec4i");

        EXPECT_TRUE(speedInt32->get<int32_t>());
        EXPECT_EQ(0, speedInt32->get<int32_t>().value());
        EXPECT_TRUE(tempFloat->get<float>());
        EXPECT_EQ(0.0f, tempFloat->get<float>().value());
        EXPECT_TRUE(nameString->get<std::string>());
        EXPECT_EQ("", nameString->get<std::string>().value());
        EXPECT_TRUE(enabledBool->get<bool>());
        EXPECT_EQ(false, *enabledBool->get<bool>());

        EXPECT_TRUE(vec_2f->get<vec2f>());
        EXPECT_TRUE(vec_3f->get<vec3f>());
        EXPECT_TRUE(vec_4f->get<vec4f>());
        EXPECT_TRUE(vec_2i->get<vec2i>());
        EXPECT_TRUE(vec_3i->get<vec3i>());
        EXPECT_TRUE(vec_4i->get<vec4i>());
        vec2f zeroVec2f{ 0.0f, 0.0f };
        vec3f zeroVec3f{ 0.0f, 0.0f, 0.0f };
        vec4f zeroVec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
        vec2i zeroVec2i{ 0, 0 };
        vec3i zeroVec3i{ 0, 0, 0 };
        vec4i zeroVec4i{ 0, 0, 0, 0 };
        EXPECT_EQ(zeroVec2f, *vec_2f->get<vec2f>());
        EXPECT_EQ(zeroVec3f, *vec_3f->get<vec3f>());
        EXPECT_EQ(zeroVec4f, *vec_4f->get<vec4f>());
        EXPECT_EQ(zeroVec2i, *vec_2i->get<vec2i>());
        EXPECT_EQ(zeroVec3i, *vec_3i->get<vec3i>());
        EXPECT_EQ(zeroVec4i, *vec_4i->get<vec4i>());
    }

}