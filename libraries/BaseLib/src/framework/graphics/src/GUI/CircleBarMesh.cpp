#include "aex/gui/CircleBarMesh.hpp"
#include "aex/gui/Frame.hpp"
#include "aex/gui/TriangleBatch.hpp"
#include "aex/gui/ShaderManager.hpp"
#include <aex/ShaderDrw.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/AexRenderTree.hpp>
#include <aex/ShaderProgram.hpp>
namespace aex
{
    namespace gui
    {
        // if u wanna macro here use \n before it. eg: \n#define MACRO 123
        namespace {
            const char* ShaderName = "CircleBarMesh";
            const char* Attribs = STRINGIFY(
                    attribute vec4 aVertex2; \n
                    );

            const char* VSH = STRINGIFY(
                    uniform vec2 uFrame; \n
                    varying vec4 vPosition; \n

                    vec2 gui2OGLPos(vec2 vec, vec2 frame)
                    {
                    vec2 retVec;
                    retVec.x = 2.0 * (vec.x / frame.x) - 1.0; \n
                    retVec.y = -(2.0 * (vec.y / frame.y) - 1.0); \n

                    return retVec;
                    }

                    vec2 gui2OGLLen(vec2 vec, vec2 frame)
                    {
                    vec2 retVec;
                    retVec.x = 2.0 * (vec.x / frame.x) - 1.0; \n
                    retVec.y = -(2.0 * (vec.y / frame.y) - 1.0); \n

                    return retVec;
                    }

                    void main()
                    {
                        \n
                            vec4 oglVertex = aVertex2; \n
                            vPosition = oglVertex; \n
                            oglVertex.xy = gui2OGLPos(aVertex2.xy, uFrame); \n
                            gl_Position = oglVertex; \n
                    }
                    );

                    const char* FSH = STRINGIFY(
                            uniform vec4 uBarColor; \n
                            uniform vec4 uBackColor; \n
                            uniform vec2 uFrame; \n
                            uniform vec2 uSmallBigR; \n
                            uniform vec2 uCenter; \n

                            uniform vec2 uPercents;
                            uniform float uBlurrPercent;
                            \n
                            varying vec4 vPosition; \n

                            vec2 gui2OGLPos(vec2 vec, vec2 frame)
                            {
                            vec2 retVec;
                            retVec.x = 2.0 * (vec.x / frame.x) - 1.0; \n
                            retVec.y = -(2.0 * (vec.y / frame.y) - 1.0); \n

                            return retVec;
                            }

                    vec2 gui2OGLLen(vec2 length, vec2 frame)
                    {
                        vec2 retVec;
                        retVec.x = 2.0 * (length.x / frame.x); \n
                            retVec.y = 2.0 * (length.y / frame.y); \n

                            return retVec;
                    }

                    int isLeft(vec2 a, vec2 b, vec2 p)
                    {
                        if ((((b.x - a.x)*(p.y - a.y)) - ((b.y - a.y)*(p.x - a.x))) >= 0.0)
                        {
                            return 0;
                        } else
                        {

                            return 1;
                        }
                    }

                    void main()
                    {
                        \n
                            vec2 tresh = uSmallBigR;
                        float dist = distance(uCenter, vPosition.xy); \n
                            float alpha = 0.0;
                        vec4 color = uBackColor; \n
                            if (dist >= tresh.x) \n // small distance
                            {
                                \n
                                    if (dist <= tresh.y) \n // max distance
                                    {
                                        \n
                                            vec2 topP = uCenter; \n
                                            topP.y = topP.y - uSmallBigR.y; \n
                                            topP = topP - uCenter; \n
                                            topP = normalize(topP); \n

                                            vec2 tmpP = vPosition.xy; \n
                                            tmpP = tmpP - uCenter; \n
                                            tmpP = normalize(tmpP); \n

                                            float startAngleRad = ((uPercents.y / 100.0) * 360.0)*0.0174532925;
                                        float angleRad = ((uPercents.x / 100.0) * 360.0)*0.0174532925;

                                        vec2 startVec;
                                        startVec.x = (topP.x * cos(startAngleRad)) - (topP.y * sin(startAngleRad));
                                        startVec.y = (topP.x * sin(startAngleRad)) + (topP.y * cos(startAngleRad));
                                        startVec = normalize(startVec);

                                        vec2 endVec;
                                        endVec.x = (topP.x * cos(angleRad + startAngleRad)) - (topP.y * sin(angleRad + startAngleRad));
                                        endVec.y = (topP.x * sin(angleRad + startAngleRad)) + (topP.y * cos(angleRad + startAngleRad));
                                        endVec = normalize(endVec);

                                        vec2 center = vec2(0.0, 0.0);
                                        float thick = tresh.y - tresh.x;
                                        float centerTresh = ((tresh.y + tresh.x) / 2.0);
                                        float blurPerc = uBlurrPercent; // default 0.1

                                        if (dist <= centerTresh)
                                        {
                                            alpha = smoothstep(tresh.x, tresh.x + (thick * blurPerc), dist);
                                        } else
                                        {
                                            alpha = 1.0 - smoothstep(tresh.y - (thick * blurPerc), tresh.y, dist);
                                        }


                                        //                      float angleRadToStartVec = acos(dot(startVec, tmpP));
                                        //                      float angleRadToEndVec = acos(dot(endVec, tmpP));
                                        //                      float angleDeg = acos( dot(topP, tmpP))*57.2957795;   \n

                                        int left1 = isLeft(center, startVec, tmpP);
                                        int left2 = isLeft(center, endVec, tmpP);


                                        if (angleRad <= (180.0 * 0.0174532925))
                                        {
                                            if ((left1 != 1) && (left2 == 1))
                                            {
                                                color = uBarColor;
                                            }
                                        } else
                                        {
                                            if ((left1 != 1))
                                            {
                                                color = uBarColor;
                                            } else
                                                if (left2 == 1)
                                                {
                                                    color = uBarColor;
                                                } else
                                                    if (uPercents.x == 100.0)
                                                    {
                                                        color = uBarColor;
                                                    }
                                        }

                                    }
                                \n
                            }
                        \n
                            \n
                            gl_FragColor = vec4(color.rgb, color.a*alpha); \n
                    }
                    \n
                );
        }


        CircleBarMesh::CircleBarMesh(const Area& area, const Color& color1, const Color& color2, float thickness, float percent, float startPercent) :
            Mesh(TriangleBatch(area)),
            m_area(area),
            m_barColor(color1),
            m_backgroundColor(color2),
            m_thickness(thickness),
            m_smallR(std::max(0.0f, (area.s().h() / 2.0f) - thickness)),
            m_bigR((area.s().h() / 2.0f)),
            m_percent(percent),
            m_startPercent(startPercent),
            m_uBarColor(color1.getClampfR(), color1.getClampfG(), color1.getClampfB(), color1.getClampfA(), "uBarColor"),
            m_uBackColor(color2.getClampfR(), color2.getClampfG(), color2.getClampfB(), color2.getClampfA(), "uBackColor"),
            m_uSmallBigR(aex::math::Vector2(m_smallR, m_bigR), "uSmallBigR"),
            m_uCenter(aex::math::Vector2(m_area.center().x(), m_area.center().y()), "uCenter"),
            m_uPercents(aex::math::Vector2(percent, startPercent), "uPercents"),
            m_uBlurrPercent(0.1, "uBlurrPercent"),
            m_shaderHandle(ShaderManager::get(ShaderName, VSH, FSH, Attribs))
        {
            m_shader = m_shaderHandle.get();
            setArea(m_area);
            batchChanged();
        }

        CircleBarMesh::~CircleBarMesh()
        {
        }

        void CircleBarMesh::setBarColor(const Color& color)
        {
            m_barColor = color;
            m_uBarColor.setValue(color.getClampfR(), color.getClampfG(), color.getClampfB(), color.getClampfA());
        }

        void CircleBarMesh::setBackgroundColor(const Color& color)
        {
            m_backgroundColor = color;
            m_uBackColor.setValue(color.getClampfR(), color.getClampfG(), color.getClampfB(), color.getClampfA());
        }

        void CircleBarMesh::setSmallR(float smallR)
        {
            m_smallR = smallR;
            m_uSmallBigR.setValue(aex::math::Vector2(m_smallR, m_bigR));
        }

        void CircleBarMesh::setBigR(float bigR)
        {
            m_bigR = bigR;
            m_uSmallBigR.setValue(aex::math::Vector2(m_smallR, m_bigR));
        }

        void CircleBarMesh::setArea(const Area& area)
        {
            m_batch.refillBatch(m_area = area);

            setSmallR((area.s().h() / 2.0f) - m_thickness);
            setBigR((area.s().h() / 2.0f));

            m_uCenter.setValue(aex::math::Vector2(m_area.center().x(), m_area.center().y()));
            setBatch(m_batch);
        }

        void CircleBarMesh::setStartPercent(float p)
        {
            m_startPercent = p;
            m_uPercents.setValue(aex::math::Vector2(m_percent, m_startPercent));
        }

        void CircleBarMesh::setPercent(float p)
        {
            m_percent = p;
            m_uPercents.setValue(aex::math::Vector2(m_percent, m_startPercent));
        }

        void CircleBarMesh::setBlurrPercent(float percent0_to_1)
        {
            m_blurrPercent = percent0_to_1;
            m_uBlurrPercent.setValue(percent0_to_1);
        }

        float CircleBarMesh::getBlurrPercent() const
        {
            return m_blurrPercent;
        }

        void CircleBarMesh::batchChanged()
        {

        }

        void CircleBarMesh::draw(AexRenderer* gl)
        {
            m_shader->draw(gl);
            const AexShader& program = m_shader->GetProgram();

            program->setVertexData(m_aVertex2);
            program->setIndices(m_indexes);

            m_uBarColor.setUniform(program);
            m_uBackColor.setUniform(program);
            m_uFrame.setUniform(program);
            m_uSmallBigR.setUniform(program);
            m_uCenter.setUniform(program);
            m_uPercents.setUniform(program);
            m_uBlurrPercent.setUniform(program);

            program->draw();
        }

        void CircleBarMesh::finishdraw(AexRenderer* gl)
        {
            const AexShader& program = m_shader->GetProgram();
            m_shader->finishdraw(gl);
            program->releaseVertexData(m_aVertex2);
        }

        void CircleBarMesh::needsUpdate()
        {
        }
    }
}
