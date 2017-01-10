using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;
using System.Drawing;
using Assimp;

namespace ResourceConverter
{
    struct FontEntry
    {
        public byte id;
        public byte x;
        public byte y;
        public byte h;
        public byte w;
    }

    class Program
    {
        public const uint TEXTURE_RAW_HEADER_LENGTH = 0x05;
        public const int MESH_HEADER_LENGTH = 0x02;
        public const int FONT_HEADER_LENGTH = 0x01;

        public const uint TEXTURE_TYPE_RAW = 0x00;
        public const uint TEXTURE_TYPE_PVR = 0x01;

        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                MessageBox.Show("Drop a resource file to be converted.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                string filename = args[0], filetype;

                // Get the file type and start the appropriate conversion
                filetype = filename.Split('.')[1];

                if (filetype.ToLower().Contains("png"))
                {
                    ImageToRaw(new Bitmap(filename), filename);
                }
                else if (filetype.ToLower().Contains("obj") || 
                         filetype.ToLower().Contains("fbx") || 
                         filetype.ToLower().Contains("blend") ||
                         filetype.ToLower().Contains("dae"))
                {
                    MeshToStaticModel(filename);
                }
                else if (filetype.ToLower().Contains("fnt"))
                {
                    GenerateFontDescriptor(filename);
                }
            }
        }

        public static void GenerateFontDescriptor(string file)
        {
            string[] fnt = File.ReadAllLines(file);
            List<FontEntry> chrList = new List<FontEntry>();

            for (int i = 4; i < fnt.Length; ++i)
            {
                string ln = fnt[i];

                if (ln.Contains("char"))
                {
                    FontEntry et = new FontEntry();

                    // Find the char id
                    int idStart, idEnd;
                    string idStr;
                    idStart = ln.IndexOf("id");
                    idStr = ln.Substring(idStart);
                    idEnd = idStr.IndexOf(" ");
                    idStr = idStr.Substring(0, idEnd);

                    et.id = byte.Parse(idStr.Split('=')[1]);

                    // Find the char x
                    int xStart, xEnd;
                    string xStr;
                    xStart = ln.IndexOf("x");
                    xStr = ln.Substring(xStart);
                    xEnd = xStr.IndexOf(" ");
                    xStr = xStr.Substring(0, xEnd);

                    et.x = byte.Parse(xStr.Split('=')[1]);

                    // Find the char y
                    int yStart, yEnd;
                    string yStr;
                    yStart = ln.IndexOf("y");
                    yStr = ln.Substring(yStart);
                    yEnd = yStr.IndexOf(" ");
                    yStr = yStr.Substring(0, yEnd);

                    et.y = byte.Parse(yStr.Split('=')[1]);

                    // Find the char width
                    int widthStart, widthEnd;
                    string widthStr;
                    widthStart = ln.IndexOf("width");
                    widthStr = ln.Substring(widthStart);
                    widthEnd = widthStr.IndexOf(" ");
                    widthStr = widthStr.Substring(0, widthEnd);

                    et.w = byte.Parse(widthStr.Split('=')[1]);

                    // Find the char height
                    int heightStart, heightEnd;
                    string heightStr;
                    heightStart = ln.IndexOf("height");
                    heightStr = ln.Substring(heightStart);
                    heightEnd = heightStr.IndexOf(" ");
                    heightStr = heightStr.Substring(0, heightEnd);

                    et.h = byte.Parse(heightStr.Split('=')[1]);

                    chrList.Add(et);
                }
            }

            // Write all the entries into the binary file
            int binLength = FONT_HEADER_LENGTH + 5 * chrList.Count;
            byte[] buf = new byte[binLength];

            // Write the header
            buf[0] = (byte)chrList.Count;

            // Write all the font entries
            for (int i = 0, offset = FONT_HEADER_LENGTH; i < chrList.Count; ++i, offset += 5)
            {
                byte[] tmp = new byte[5];

                tmp[0] = chrList[i].id;
                tmp[1] = chrList[i].x;
                tmp[2] = chrList[i].y;
                tmp[3] = chrList[i].h;
                tmp[4] = chrList[i].w;

                Buffer.BlockCopy(tmp, 0, buf, offset, 5);
            }

            File.WriteAllBytes(file.Split('.')[0] + ".binfnt", buf);
        }

        public static void MeshToStaticModel(string file)
        {
            AssimpImporter importer = new AssimpImporter();

            try
            {
                Scene scene = importer.ImportFile(file, PostProcessSteps.Triangulate);

                if (scene.MeshCount == 1)
                {
                    Vector3D[] vertices = scene.Meshes[0].Vertices;
                    Vector3D[] uvCoords = scene.Meshes[0].GetTextureCoords(0);
                    Vector3D[] normals = null;
                    short[] indices = scene.Meshes[0].GetShortIndices();

                    if (!scene.Meshes[0].HasNormals)
                    {
                        MessageBox.Show("Mesh doesn't have normals. Check if the mesh has been exported correctly.",
                            "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        Application.Exit();
                    }

                    normals = scene.Meshes[0].Normals;

                    // 4 byte floats * 3 dimensions
                    byte[] vertexData = new byte[12 * vertices.Length];

                    // 4 byte floats * 2 dimensions
                    byte[] uvData = new byte[8 * uvCoords.Length];

                    // 4 byte normals * 3 dimensions
                    byte[] normalData = new byte[12 * normals.Length];

                    int vertexOffset = 0, uvOffset = 0, normalOffset = 0;

                    // Extract vertex data
                    for (int i = 0; i < vertices.Length; ++i)
                    {
                        byte[] x, y, z;
                        x = BitConverter.GetBytes(vertices[i].X);
                        y = BitConverter.GetBytes(vertices[i].Y);
                        z = BitConverter.GetBytes(vertices[i].Z);

                        Buffer.BlockCopy(x, 0, vertexData, vertexOffset, 4);
                        Buffer.BlockCopy(y, 0, vertexData, vertexOffset + 4, 4);
                        Buffer.BlockCopy(z, 0, vertexData, vertexOffset + 8, 4);

                        vertexOffset += 12;
                    }

                    // Extract UV coordinates data
                    for (int i = 0; i < uvCoords.Length; ++i)
                    {
                        byte[] s, t;
                        s = BitConverter.GetBytes(uvCoords[i].X);
                        t = BitConverter.GetBytes(uvCoords[i].Y);

                        Buffer.BlockCopy(s, 0, uvData, uvOffset, 4);
                        Buffer.BlockCopy(t, 0, uvData, uvOffset + 4, 4);

                        uvOffset += 8;
                    }

                    // Extract normal data
                    for (int i = 0; i < normals.Length; ++i)
                    {
                        byte[] x, y, z;
                        x = BitConverter.GetBytes(normals[i].X);
                        y = BitConverter.GetBytes(normals[i].Y);
                        z = BitConverter.GetBytes(normals[i].Z);

                        Buffer.BlockCopy(x, 0, normalData, normalOffset, 4);
                        Buffer.BlockCopy(y, 0, normalData, normalOffset + 4, 4);
                        Buffer.BlockCopy(z, 0, normalData, normalOffset + 8, 4);

                        normalOffset += 12;
                    }

                    // Save the raw mesh file
                    byte[] binary = new byte[MESH_HEADER_LENGTH + vertexData.Length + uvData.Length + normalData.Length + 2 * indices.Length];

                    // Set the vertex count
                    binary[0] = BitConverter.GetBytes((UInt16)vertices.Length)[0];
                    binary[1] = BitConverter.GetBytes((UInt16)vertices.Length)[1];

                    // Copy the vertices
                    Buffer.BlockCopy(vertexData, 0, binary, MESH_HEADER_LENGTH, vertexData.Length);

                    // Copy the UV coordinates
                    Buffer.BlockCopy(uvData, 0, binary, MESH_HEADER_LENGTH + vertexData.Length, uvData.Length);

                    // Copy the normals
                    Buffer.BlockCopy(normalData, 0, binary, MESH_HEADER_LENGTH + vertexData.Length + uvData.Length, normalData.Length);

                    // Copy the indices
                    Buffer.BlockCopy(indices, 0, binary, MESH_HEADER_LENGTH + vertexData.Length + uvData.Length + normalData.Length, 2 * indices.Length);

                    // Save the converted file
                    File.WriteAllBytes(file.Split('.')[0] + ".mesh", binary);
                }
                else
                {
                    MessageBox.Show("Model has " + scene.MeshCount + 
                        " submeshes. Only models with one submesh can be processed", 
                        "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            catch (AssimpException assExp)
            {
                MessageBox.Show(assExp.Message, "Error converting mesh", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public static void ImageToRaw(Bitmap img, string file)
        {
            byte[] binary = new byte[img.Width * img.Height * 3 + TEXTURE_RAW_HEADER_LENGTH];
            uint index = TEXTURE_RAW_HEADER_LENGTH;

            // Set image data
            for (int y = img.Height - 1; y >= 0; y--)
            {
                for (int x = 0; x < img.Width; x++)
                {
                    Color cl = img.GetPixel(x, y);
                    binary[index++] = cl.R;
                    binary[index++] = cl.G;
                    binary[index++] = cl.B;
                }
            }

            // Set header data
            binary[0] = 0x00;

            binary[1] = BitConverter.GetBytes((UInt16)img.Width)[0];
            binary[2] = BitConverter.GetBytes((UInt16)img.Width)[1];

            binary[3] = BitConverter.GetBytes((UInt16)img.Height)[0];
            binary[4] = BitConverter.GetBytes((UInt16)img.Height)[1];

            // Save the converted file
            File.WriteAllBytes(file.Split('.')[0] + ".raw", binary);
        }
    }
}
