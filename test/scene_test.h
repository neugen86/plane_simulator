#ifndef SCENE_TEST_H
#define SCENE_TEST_H

#include "scene/scene.h"

using namespace scene;

TEST(SceneTest, resuming)
{
    Scene scene;

    ASSERT_TRUE(scene.start());
    ASSERT_FALSE(scene.start());
    sleep(1);

    for (int i = 0; i < 3; ++i)
    {
        ASSERT_TRUE(scene.pause());
        ASSERT_FALSE(scene.pause());
        sleep(1);

        ASSERT_TRUE(scene.start());
        ASSERT_FALSE(scene.start());
        sleep(1);
    }

    ASSERT_TRUE(scene.stop());
    ASSERT_FALSE(scene.stop());
}

TEST(SceneTest, stopping)
{
    Scene scene;

    for (int i = 0; i < 3; ++i)
    {
        ASSERT_TRUE(scene.start());
        ASSERT_FALSE(scene.start());
        sleep(1);

        ASSERT_TRUE(scene.stop());
        ASSERT_FALSE(scene.stop());
        sleep(1);
    }
}

#endif // SCENE_TEST_H
