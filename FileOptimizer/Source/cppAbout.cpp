﻿// ---------------------------------------------------------------------------
#include "cppAbout.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


// ---------------------------------------------------------------------------
TfrmAbout *frmAbout;
TImage *imgEaster = nullptr;
TTimer *tmrEaster = nullptr;


//---------------------------------------------------------------------------
__fastcall TfrmAbout::TfrmAbout(TComponent* Owner): TForm(Owner)
{
}



// ---------------------------------------------------------------------------
__fastcall TfrmAbout::~TfrmAbout()
{
}



//---------------------------------------------------------------------------
void __fastcall TfrmAbout::FormCreate(TObject *Sender)
{
	TCHAR acName[256];


	clsUtil::LoadForm(this);
	clsLanguage::TranslateForm(this);

	Icon = Application->Icon;

	lblProduct->Caption = Application->Name;
	lblProduct->Caption = lblProduct->Caption +  " " + clsUtil::ExeVersion(Application->ExeName.c_str());
	#if defined(_WIN64)
		lblProduct->Caption = lblProduct->Caption + " (x64)";
	#else
		lblProduct->Caption = lblProduct->Caption + " (x86)";
	#endif
	lblDetail->Caption = __DATE__;

	clsUtil::GetFileVersionField(Application->ExeName.c_str(), (const TCHAR *) _T("LegalCopyright"), acName, (sizeof(acName) / sizeof(TCHAR)) - 1);
	lblCopyright->Caption = acName;
	lblCopyright->Hint = KS_APP_URL;

	#if ((__BORLANDC__ >= 0x0630) && (__BORLANDC__ <= 0x0639))
		imgAbout->Hint = "Embarcadero C++ Builder XE";
	#elif ((__BORLANDC__ >= 0x0640) && (__BORLANDC__ <= 0x0649))
		imgAbout->Hint = "Embarcadero C++ Builder XE2";
	#elif ((__BORLANDC__ >= 0x0650) && (__BORLANDC__ <= 0x0659))
		imgAbout->Hint = "Embarcadero C++ Builder XE3";
	#elif ((__BORLANDC__ >= 0x0660) && (__BORLANDC__ <= 0x0669))
		imgAbout->Hint = "Embarcadero C++ Builder XE4";
	#elif ((__BORLANDC__ >= 0x0670) && (__BORLANDC__ <= 0x0679))
		imgAbout->Hint = "Embarcadero C++ Builder XE5";
	#elif ((__BORLANDC__ >= 0x0680) && (__BORLANDC__ <= 0x0689))
		imgAbout->Hint = "Embarcadero C++ Builder XE6";
	#elif ((__BORLANDC__ >= 0x0690) && (__BORLANDC__ <= 0x0699))
		imgAbout->Hint = "Embarcadero C++ Builder XE7";
	#elif ((__BORLANDC__ >= 0x0700) && (__BORLANDC__ <= 0x0709))
		imgAbout->Hint = "Embarcadero C++ Builder XE8";
	#elif ((__BORLANDC__ >= 0x0710) && (__BORLANDC__ <= 0x0719))
		imgAbout->Hint = "Embarcadero C++ Builder 10 Seattle";
	#elif ((__BORLANDC__ >= 0x0720) && (__BORLANDC__ <= 0x0729))
		imgAbout->Hint = "Embarcadero C++ Builder 10.1 Berlin";
	#elif ((__BORLANDC__ >= 0x0730) && (__BORLANDC__ <= 0x0739))
		imgAbout->Hint = "Embarcadero C++ Builder 10.2 Tokyo";
	#elif ((__BORLANDC__ >= 0x0740) && (__BORLANDC__ <= 0x0749))
		imgAbout->Hint = "Embarcadero C++ Builder 10.3 Rio";
	#elif ((__BORLANDC__ >= 0x0750) && (__BORLANDC__ <= 0x0759))
		imgAbout->Hint = "Embarcadero C++ Builder 10.4 Sydney";
	#elif ((__BORLANDC__ >= 0x0760) && (__BORLANDC__ <= 0x0769))
		imgAbout->Hint = "Embarcadero C++ Builder 11.0 Alexandria";
	#elif ((__BORLANDC__ >= 0x0770) && (__BORLANDC__ <= 0x0779))
		imgAbout->Hint = "Embarcadero C++ Builder 12.0";
	#else
		imgAbout->Hint = "Borland/Inprise/CodeGear/Embarcadero C++ Builder";
	#endif
	imgAbout->Hint += " (" + (String) __BORLANDC__ + ")";

	#if defined (__clang__)
		imgAbout->Hint += ". Clang " + (String) __clang_version__;
	#endif

	#if defined (_DEBUG)
		imgAbout->Hint += ". DEBUG";
	#endif


	static const char acRtf[] = "{\\rtf1\\ansi\\ansicpg1252\\deff0\\nouicompat\\deflang3082\\deflangfe3082{\\fonttbl{\\f0\\fswiss\\fprq2\\fcharset0 Tahoma;}{\\f1\\fnil\\fcharset0 Calibri;}{\\f2\\fnil\\fcharset2 Symbol;}}\
{\\colortbl ;\\red51\\green51\\blue51;\\red34\\green34\\blue34;\\red0\\green0\\blue255;\\red255\\green255\\blue255;}\
{\\*\\generator Riched20 10.0.17134}{\\*\\mmathPr\\mdispDef1\\mwrapIndent1440 }\\viewkind4\\uc1 \
\\pard\\cbpat4\\widctlpar\\sb288\\sa192\\qc\\cf1\\b\\f0\\fs36 GNU AFFERO GENERAL PUBLIC LICENSE\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\qc\\cf2\\b0\\fs24 Version 3, 19 November 2007\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0 Copyright \\'a9 2007 Free Software Foundation, Inc. <{\\cf0{\\field{\\*\\fldinst{HYPERLINK \"https://fsf.org/\"}}{\\fldrslt{https://fsf.org/\\ul0\\cf0}}}}\\f0\\fs24 >\\~\\line Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa192\\cf1\\b\\fs36 Preamble\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 The GNU Affero General Public License is a free, copyleft license for software and other kinds of works, specifically designed to ensure cooperation with the community in the case of network server software.\\par\
The licenses for most software and other practical works are designed to take away your freedom to share and change the works. By contrast, our General Public Licenses are intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.\\par\
When we speak of free software, we are referring to freedom, not price. Our General Public Licenses are designed to make sure that you have the freedom to distribute copies of free software (and charge for them if you wish), that you receive source code or can get it if you want it, that you can change the software or use pieces of it in new free programs, and that you know you can do these things.\\par\
Developers that use our General Public Licenses protect your rights with two steps: (1) assert copyright on the software, and (2) offer you this License which gives you legal permission to copy, distribute and/or modify the software.\\par\
A secondary benefit of defending all users' freedom is that improvements made in alternate versions of the program, if they receive widespread use, become available for other developers to incorporate. Many developers of free software are heartened and encouraged by the resulting cooperation. However, in the case of software used on network servers, this result may fail to come about. The GNU General Public License permits making a modified version and letting the public access it on a server without ever releasing its source code to the public.\\par\
The GNU Affero General Public License is designed specifically to ensure that, in such cases, the modified source code becomes available to the community. It requires the operator of a network server to provide the source code of the modified version running there to the users of that server. Therefore, public use of a modified version, on a publicly accessible server, gives the public access to the source code of the modified version.\\par\
An older license, called the Affero General Public License and published by Affero, was designed to accomplish similar goals. This is a different license, not a version of the Affero GPL, but Affero has released a new version of the Affero GPL which permits relicensing under this license.\\par\
The precise terms and conditions for copying, distribution and modification follow.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa192\\cf1\\b\\fs36 TERMS AND CONDITIONS\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\fs29 0. Definitions.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 \"This License\" refers to version 3 of the GNU Affero General Public License.\\par\
\"Copyright\" also means copyright-like laws that apply to other kinds of works, such as semiconductor masks.\\par\
\"The Program\" refers to any copyrightable work licensed under this License. Each licensee is addressed as \"you\". \"Licensees\" and \"recipients\" may be individuals or organizations.\\par\
To \"modify\" a work means to copy from or adapt all or part of the work in a fashion requiring copyright permission, other than the making of an exact copy. The resulting work is called a \"modified version\" of the earlier work or a work \"based on\" the earlier work.\\par\
A \"covered work\" means either the unmodified Program or a work based on the Program.\\par\
To \"propagate\" a work means to do anything with it that, without permission, would make you directly or secondarily liable for infringement under applicable copyright law, except executing it on a computer or modifying a private copy. Propagation includes copying, distribution (with or without modification), making available to the public, and in some countries other activities as well.\\par\
To \"convey\" a work means any kind of propagation that enables other parties to make or receive copies. Mere interaction with a user through a computer network, with no transfer of a copy, is not conveying.\\par\
An interactive user interface displays \"Appropriate Legal Notices\" to the extent that it includes a convenient and prominently visible feature that (1) displays an appropriate copyright notice, and (2) tells the user that there is no warranty for the work (except to the extent that warranties are provided), that licensees may convey the work under this License, and how to view a copy of this License. If the interface presents a list of user commands or options, such as a menu, a prominent item in the list meets this criterion.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 1. Source Code.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 The \"source code\" for a work means the preferred form of the work for making modifications to it. \"Object code\" means any non-source form of a work.\\par\
A \"Standard Interface\" means an interface that either is an official standard defined by a recognized standards body, or, in the case of interfaces specified for a particular programming language, one that is widely used among developers working in that language.\\par\
The \"System Libraries\" of an executable work include anything, other than the work as a whole, that (a) is included in the normal form of packaging a Major Component, but which is not part of that Major Component, and (b) serves only to enable use of the work with that Major Component, or to implement a Standard Interface for which an implementation is available to the public in source code form. A \"Major Component\", in this context, means a major essential component (kernel, window system, and so on) of the specific operating system (if any) on which the executable work runs, or a compiler used to produce the work, or an object code interpreter used to run it.\\par\
The \"Corresponding Source\" for a work in object code form means all the source code needed to generate, install, and (for an executable work) run the object code and to modify the work, including scripts to control those activities. However, it does not include the work's System Libraries, or general-purpose tools or generally available free programs which are used unmodified in performing those activities but which are not part of the work. For example, Corresponding Source includes interface definition files associated with source files for the work, and the source code for shared libraries and dynamically linked subprograms that the work is specifically designed to require, such as by intimate data communication or control flow between those subprograms and other parts of the work.\\par\
The Corresponding Source need not include anything that users can regenerate automatically from other parts of the Corresponding Source.\\par\
The Corresponding Source for a work in source code form is that same work.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 2. Basic Permissions.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 All rights granted under this License are granted for the term of copyright on the Program, and are irrevocable provided the stated conditions are met. This License explicitly affirms your unlimited permission to run the unmodified Program. The output from running a covered work is covered by this License only if the output, given its content, constitutes a covered work. This License acknowledges your rights of fair use or other equivalent, as provided by copyright law.\\par\
You may make, run and propagate covered works that you do not convey, without conditions so long as your license otherwise remains in force. You may convey covered works to others for the sole purpose of having them make modifications exclusively for you, or provide you with facilities for running those works, provided that you comply with the terms of this License in conveying all material for which you do not control copyright. Those thus making or running the covered works for you must do so exclusively on your behalf, under your direction and control, on terms that prohibit them from making any copies of your copyrighted material outside their relationship with you.\\par\
Conveying under any other circumstances is permitted solely under the conditions stated below. Sublicensing is not allowed; section 10 makes it unnecessary.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 3. Protecting Users' Legal Rights From Anti-Circumvention Law.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 No covered work shall be deemed part of an effective technological measure under any applicable law fulfilling obligations under article 11 of the WIPO copyright treaty adopted on 20 December 1996, or similar laws prohibiting or restricting circumvention of such measures.\\par\
When you convey a covered work, you waive any legal power to forbid circumvention of technological measures to the extent such circumvention is effected by exercising rights under this License with respect to the covered work, and you disclaim any intention to limit operation or modification of the work as a means of enforcing, against the work's users, your or third parties' legal rights to forbid circumvention of technological measures.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 4. Conveying Verbatim Copies.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 You may convey verbatim copies of the Program's source code as you receive it, in any medium, provided that you conspicuously and appropriately publish on each copy an appropriate copyright notice; keep intact all notices stating that this License and any non-permissive terms added in accord with section 7 apply to the code; keep intact all notices of the absence of any warranty; and give all recipients a copy of this License along with the Program.\\par\
You may charge any price or no price for each copy that you convey, and you may offer support or warranty protection for a fee.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 5. Conveying Modified Source Versions.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 You may convey a work based on the Program, or the modifications to produce it from the Program, in the form of source code under the terms of section 4, provided that you also meet all of these conditions:\\par\
\
\\pard\\cbpat4{\\pntext\\f2\\'B7\\tab}{\\*\\pn\\pnlvlblt\\pnf2\\pnindent587{\\pntxtb\\'B7}}\\fi-360\\li493\\ri253\\sb120\\sl360\\slmult0 a) The work must carry prominent notices stating that you modified it, and giving a relevant date.\\par\
{\\pntext\\f2\\'B7\\tab}b) The work must carry prominent notices stating that it is released under this License and any conditions added under section 7. This requirement modifies the requirement in section 4 to \"keep intact all notices\".\\par\
{\\pntext\\f2\\'B7\\tab}c) You must license the entire work, as a whole, under this License to anyone who comes into possession of a copy. This License will therefore apply, along with any applicable section 7 additional terms, to the whole of the work, and all its parts, regardless of how they are packaged. This License gives no permission to license the work in any other way, but it does not invalidate such permission if you have separately received it.\\par\
{\\pntext\\f2\\'B7\\tab}d) If the work has interactive user interfaces, each must display Appropriate Legal Notices; however, if the Program has interactive interfaces that do not display Appropriate Legal Notices, your work need not make them do so.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0 A compilation of a covered work with other separate and independent works, which are not by their nature extensions of the covered work, and which are not combined with it such as to form a larger program, in or on a volume of a storage or distribution medium, is called an \"aggregate\" if the compilation and its resulting copyright are not used to limit the access or legal rights of the compilation's users beyond what the individual works permit. Inclusion of a covered work in an aggregate does not cause this License to apply to the other parts of the aggregate.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 6. Conveying Non-Source Forms.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 You may convey a covered work in object code form under the terms of sections 4 and 5, provided that you also convey the machine-readable Corresponding Source under the terms of this License, in one of these ways:\\par\
\
\\pard\\cbpat4{\\pntext\\f2\\'B7\\tab}{\\*\\pn\\pnlvlblt\\pnf2\\pnindent587{\\pntxtb\\'B7}}\\fi-360\\li493\\ri253\\sb120\\sl360\\slmult0 a) Convey the object code in, or embodied in, a physical product (including a physical distribution medium), accompanied by the Corresponding Source fixed on a durable physical medium customarily used for software interchange.\\par\
{\\pntext\\f2\\'B7\\tab}b) Convey the object code in, or embodied in, a physical product (including a physical distribution medium), accompanied by a written offer, valid for at least three years and valid for as long as you offer spare parts or customer support for that product model, to give anyone who possesses the object code either (1) a copy of the Corresponding Source for all the software in the product that is covered by this License, on a durable physical medium customarily used for software interchange, for a price no more than your reasonable cost of physically performing this conveying of source, or (2) access to copy the Corresponding Source from a network server at no charge.\\par\
{\\pntext\\f2\\'B7\\tab}c) Convey individual copies of the object code with a copy of the written offer to provide the Corresponding Source. This alternative is allowed only occasionally and noncommercially, and only if you received the object code with such an offer, in accord with subsection 6b.\\par\
{\\pntext\\f2\\'B7\\tab}d) Convey the object code by offering access from a designated place (gratis or for a charge), and offer equivalent access to the Corresponding Source in the same way through the same place at no further charge. You need not require recipients to copy the Corresponding Source along with the object code. If the place to copy the object code is a network server, the Corresponding Source may be on a different server (operated by you or a third party) that supports equivalent copying facilities, provided you maintain clear directions next to the object code saying where to find the Corresponding Source. Regardless of what server hosts the Corresponding Source, you remain obligated to ensure that it is available for as long as needed to satisfy these requirements.\\par\
{\\pntext\\f2\\'B7\\tab}e) Convey the object code using peer-to-peer transmission, provided you inform other peers where the object code and Corresponding Source of the work are being offered to the general public at no charge under subsection 6d.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0 A separable portion of the object code, whose source code is excluded from the Corresponding Source as a System Library, need not be included in conveying the object code work.\\par\
A \"User Product\" is either (1) a \"consumer product\", which means any tangible personal property which is normally used for personal, family, or household purposes, or (2) anything designed or sold for incorporation into a dwelling. In determining whether a product is a consumer product, doubtful cases shall be resolved in favor of coverage. For a particular product received by a particular user, \"normally used\" refers to a typical or common use of that class of product, regardless of the status of the particular user or of the way in which the particular user actually uses, or expects or is expected to use, the product. A product is a consumer product regardless of whether the product has substantial commercial, industrial or non-consumer uses, unless such uses represent the only significant mode of use of the product.\\par\
\"Installation Information\" for a User Product means any methods, procedures, authorization keys, or other information required to install and execute modified versions of a covered work in that User Product from a modified version of its Corresponding Source. The information must suffice to ensure that the continued functioning of the modified object code is in no case prevented or interfered with solely because modification has been made.\\par\
If you convey an object code work under this section in, or with, or specifically for use in, a User Product, and the conveying occurs as part of a transaction in which the right of possession and use of the User Product is transferred to the recipient in perpetuity or for a fixed term (regardless of how the transaction is characterized), the Corresponding Source conveyed under this section must be accompanied by the Installation Information. But this requirement does not apply if neither you nor any third party retains the ability to install modified object code on the User Product (for example, the work has been installed in ROM).\\par\
The requirement to provide Installation Information does not include a requirement to continue to provide support service, warranty, or updates for a work that has been modified or installed by the recipient, or for the User Product in which it has been modified or installed. Access to a network may be denied when the modification itself materially and adversely affects the operation of the network or violates the rules and protocols for communication across the network.\\par\
Corresponding Source conveyed, and Installation Information provided, in accord with this section must be in a format that is publicly documented (and with an implementation available to the public in source code form), and must require no special password or key for unpacking, reading or copying.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 7. Additional Terms.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 \"Additional permissions\" are terms that supplement the terms of this License by making exceptions from one or more of its conditions. Additional permissions that are applicable to the entire Program shall be treated as though they were included in this License, to the extent that they are valid under applicable law. If additional permissions apply only to part of the Program, that part may be used separately under those permissions, but the entire Program remains governed by this License without regard to the additional permissions.\\par\
When you convey a copy of a covered work, you may at your option remove any additional permissions from that copy, or from any part of it. (Additional permissions may be written to require their own removal in certain cases when you modify the work.) You may place additional permissions on material, added by you to a covered work, for which you have or can give appropriate copyright permission.\\par\
Notwithstanding any other provision of this License, for material you add to a covered work, you may (if authorized by the copyright holders of that material) supplement the terms of this License with terms:\\par\
\
\\pard\\cbpat4{\\pntext\\f2\\'B7\\tab}{\\*\\pn\\pnlvlblt\\pnf2\\pnindent587{\\pntxtb\\'B7}}\\fi-360\\li493\\ri253\\sb120\\sl360\\slmult0 a) Disclaiming warranty or limiting liability differently from the terms of sections 15 and 16 of this License; or\\par\
{\\pntext\\f2\\'B7\\tab}b) Requiring preservation of specified reasonable legal notices or author attributions in that material or in the Appropriate Legal Notices displayed by works containing it; or\\par\
{\\pntext\\f2\\'B7\\tab}c) Prohibiting misrepresentation of the origin of that material, or requiring that modified versions of such material be marked in reasonable ways as different from the original version; or\\par\
{\\pntext\\f2\\'B7\\tab}d) Limiting the use for publicity purposes of names of licensors or authors of the material; or\\par\
{\\pntext\\f2\\'B7\\tab}e) Declining to grant rights under trademark law for use of some trade names, trademarks, or service marks; or\\par\
{\\pntext\\f2\\'B7\\tab}f) Requiring indemnification of licensors and authors of that material by anyone who conveys the material (or modified versions of it) with contractual assumptions of liability to the recipient, for any liability that these contractual assumptions directly impose on those licensors and authors.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0 All other non-permissive additional terms are considered \"further restrictions\" within the meaning of section 10. If the Program as you received it, or any part of it, contains a notice stating that it is governed by this License along with a term that is a further restriction, you may remove that term. If a license document contains a further restriction but permits relicensing or conveying under this License, you may add to a covered work material governed by the terms of that license document, provided that the further restriction does not survive such relicensing or conveying.\\par\
If you add terms to a covered work in accord with this section, you must place, in the relevant source files, a statement of the additional terms that apply to those files, or a notice indicating where to find the applicable terms.\\par\
Additional terms, permissive or non-permissive, may be stated in the form of a separately written license, or stated as exceptions; the above requirements apply either way.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 8. Termination.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 You may not propagate or modify a covered work except as expressly provided under this License. Any attempt otherwise to propagate or modify it is void, and will automatically terminate your rights under this License (including any patent licenses granted under the third paragraph of section 11).\\par\
However, if you cease all violation of this License, then your license from a particular copyright holder is reinstated (a) provisionally, unless and until the copyright holder explicitly and finally terminates your license, and (b) permanently, if the copyright holder fails to notify you of the violation by some reasonable means prior to 60 days after the cessation.\\par\
Moreover, your license from a particular copyright holder is reinstated permanently if the copyright holder notifies you of the violation by some reasonable means, this is the first time you have received notice of violation of this License (for any work) from that copyright holder, and you cure the violation prior to 30 days after your receipt of the notice.\\par\
Termination of your rights under this section does not terminate the licenses of parties who have received copies or rights from you under this License. If your rights have been terminated and not permanently reinstated, you do not qualify to receive new licenses for the same material under section 10.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 9. Acceptance Not Required for Having Copies.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 You are not required to accept this License in order to receive or run a copy of the Program. Ancillary propagation of a covered work occurring solely as a consequence of using peer-to-peer transmission to receive a copy likewise does not require acceptance. However, nothing other than this License grants you permission to propagate or modify any covered work. These actions infringe copyright if you do not accept this License. Therefore, by modifying or propagating a covered work, you indicate your acceptance of this License to do so.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 10. Automatic Licensing of Downstream Recipients.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 Each time you convey a covered work, the recipient automatically receives a license from the original licensors, to run, modify and propagate that work, subject to this License. You are not responsible for enforcing compliance by third parties with this License.\\par\
An \"entity transaction\" is a transaction transferring control of an organization, or substantially all assets of one, or subdividing an organization, or merging organizations. If propagation of a covered work results from an entity transaction, each party to that transaction who receives a copy of the work also receives whatever licenses to the work the party's predecessor in interest had or could give under the previous paragraph, plus a right to possession of the Corresponding Source of the work from the predecessor in interest, if the predecessor has it or can get it with reasonable efforts.\\par\
You may not impose any further restrictions on the exercise of the rights granted or affirmed under this License. For example, you may not impose a license fee, royalty, or other charge for exercise of rights granted under this License, and you may not initiate litigation (including a cross-claim or counterclaim in a lawsuit) alleging that any patent claim is infringed by making, using, selling, offering for sale, or importing the Program or any portion of it.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 11. Patents.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 A \"contributor\" is a copyright holder who authorizes use under this License of the Program or a work on which the Program is based. The work thus licensed is called the contributor's \"contributor version\".\\par\
A contributor's \"essential patent claims\" are all patent claims owned or controlled by the contributor, whether already acquired or hereafter acquired, that would be infringed by some manner, permitted by this License, of making, using, or selling its contributor version, but do not include claims that would be infringed only as a consequence of further modification of the contributor version. For purposes of this definition, \"control\" includes the right to grant patent sublicenses in a manner consistent with the requirements of this License.\\par\
Each contributor grants you a non-exclusive, worldwide, royalty-free patent license under the contributor's essential patent claims, to make, use, sell, offer for sale, import and otherwise run, modify and propagate the contents of its contributor version.\\par\
In the following three paragraphs, a \"patent license\" is any express agreement or commitment, however denominated, not to enforce a patent (such as an express permission to practice a patent or covenant not to sue for patent infringement). To \"grant\" such a patent license to a party means to make such an agreement or commitment not to enforce a patent against the party.\\par\
If you convey a covered work, knowingly relying on a patent license, and the Corresponding Source of the work is not available for anyone to copy, free of charge and under the terms of this License, through a publicly available network server or other readily accessible means, then you must either (1) cause the Corresponding Source to be so available, or (2) arrange to deprive yourself of the benefit of the patent license for this particular work, or (3) arrange, in a manner consistent with the requirements of this License, to extend the patent license to downstream recipients. \"Knowingly relying\" means you have actual knowledge that, but for the patent license, your conveying the covered work in a country, or your recipient's use of the covered work in a country, would infringe one or more identifiable patents in that country that you have reason to believe are valid.\\par\
If, pursuant to or in connection with a single transaction or arrangement, you convey, or propagate by procuring conveyance of, a covered work, and grant a patent license to some of the parties receiving the covered work authorizing them to use, propagate, modify or convey a specific copy of the covered work, then the patent license you grant is automatically extended to all recipients of the covered work and works based on it.\\par\
A patent license is \"discriminatory\" if it does not include within the scope of its coverage, prohibits the exercise of, or is conditioned on the non-exercise of one or more of the rights that are specifically granted under this License. You may not convey a covered work if you are a party to an arrangement with a third party that is in the business of distributing software, under which you make payment to the third party based on the extent of your activity of conveying the work, and under which the third party grants, to any of the parties who would receive the covered work from you, a discriminatory patent license (a) in connection with copies of the covered work conveyed by you (or copies made from those copies), or (b) primarily for and in connection with specific products or compilations that contain the covered work, unless you entered into that arrangement, or that patent license was granted, prior to 28 March 2007.\\par\
Nothing in this License shall be construed as excluding or limiting any implied license or other defenses to infringement that may otherwise be available to you under applicable patent law.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 12. No Surrender of Others' Freedom.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 If conditions are imposed on you (whether by court order, agreement or otherwise) that contradict the conditions of this License, they do not excuse you from the conditions of this License. If you cannot convey a covered work so as to satisfy simultaneously your obligations under this License and any other pertinent obligations, then as a consequence you may not convey it at all. For example, if you agree to terms that obligate you to collect a royalty for further conveying from those to whom you convey the Program, the only way you could satisfy both those terms and this License would be to refrain entirely from conveying the Program.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 13. Remote Network Interaction; Use with the GNU General Public License.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 Notwithstanding any other provision of this License, if you modify the Program, your modified version must prominently offer all users interacting with it remotely through a computer network (if your version supports such interaction) an opportunity to receive the Corresponding Source of your version by providing access to the Corresponding Source from a network server at no charge, through some standard or customary means of facilitating copying of software. This Corresponding Source shall include the Corresponding Source for any work covered by version 3 of the GNU General Public License that is incorporated pursuant to the following paragraph.\\par\
Notwithstanding any other provision of this License, you have permission to link or combine any covered work with a work licensed under version 3 of the GNU General Public License into a single combined work, and to convey the resulting work. The terms of this License will continue to apply to the part which is the covered work, but the work with which it is combined will remain governed by version 3 of the GNU General Public License.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 14. Revised Versions of this License.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 The Free Software Foundation may publish revised and/or new versions of the GNU Affero General Public License from time to time. Such new versions will be similar in spirit to the present version, but may differ in detail to address new problems or concerns.\\par\
Each version is given a distinguishing version number. If the Program specifies that a certain numbered version of the GNU Affero General Public License \"or any later version\" applies to it, you have the option of following the terms and conditions either of that numbered version or of any later version published by the Free Software Foundation. If the Program does not specify a version number of the GNU Affero General Public License, you may choose any version ever published by the Free Software Foundation.\\par\
If the Program specifies that a proxy can decide which future versions of the GNU Affero General Public License can be used, that proxy's public statement of acceptance of a version permanently authorizes you to choose that version for the Program.\\par\
Later license versions may give you additional or different permissions. However, no additional obligations are imposed on any author or copyright holder as a result of your choosing to follow a later version.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 15. Disclaimer of Warranty.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 16. Limitation of Liability.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa199\\cf1\\b\\fs29 17. Interpretation of Sections 15 and 16.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 If the disclaimer of warranty and limitation of liability provided above cannot be given local legal effect according to their terms, reviewing courts shall apply local law that most closely approximates an absolute waiver of all civil liability in connection with the Program, unless a warranty or assumption of liability accompanies a copy of the Program in return for a fee.\\par\
END OF TERMS AND CONDITIONS\\par\
\
\\pard\\cbpat4\\widctlpar\\sb288\\sa192\\cf1\\b\\fs36 How to Apply These Terms to Your New Programs\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\cf2\\b0\\fs24 If you develop a new program, and you want it to be of the greatest possible use to the public, the best way to achieve this is to make it free software which everyone can redistribute and change under these terms.\\par\
To do so, attach the following notices to the program. It is safest to attach them to the start of each source file to most effectively state the exclusion of warranty; and each file should have at least the \"copyright\" line and a pointer to where the full notice is found.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\tx916\\tx1832\\tx2748\\tx3664\\tx4580\\tx5496\\tx6412\\tx7328\\tx8244\\tx9160\\tx10076\\tx10992\\tx11908\\tx12824\\tx13740\\tx14656\\fs20     <one line to give the program's name and a brief idea of what it does.>\\par\
    Copyright (C) <year>  <name of author>\\par\
\\par\
    This program is free software: you can redistribute it and/or modify\\par\
    it under the terms of the GNU Affero General Public License as\\par\
    published by the Free Software Foundation, either version 3 of the\\par\
    License, or (at your option) any later version.\\par\
\\par\
    This program is distributed in the hope that it will be useful,\\par\
    but WITHOUT ANY WARRANTY; without even the implied warranty of\\par\
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\\par\
    GNU Affero General Public License for more details.\\par\
\\par\
    You should have received a copy of the GNU Affero General Public License\\par\
    along with this program.  If not, see <{\\cf0{\\field{\\*\\fldinst{HYPERLINK \"https://www.gnu.org/licenses/\"}}{\\fldrslt{https://www.gnu.org/licenses/\\ul0\\cf0}}}}\\f0\\fs20 >.\\par\
\
\\pard\\cbpat4\\widctlpar\\sb240\\sa240\\sl360\\slmult0\\fs24 Also add information on how to contact you by electronic and paper mail.\\par\
If your software can interact with users remotely through a computer network, you should also make sure that it provides a way for users to get its source. For example, if your program is a web application, its interface could display a \"Source\" link that leads users to an archive of the code. There are many ways you could offer source, and different solutions will be better for different programs; see section 13 for the specific requirements.\\par\
You should also get your employer (if you work as a programmer) or school, if any, to sign a \"copyright disclaimer\" for the program, if necessary. For more information on this, and how to apply and follow the GNU AGPL, see <{\\cf0{\\field{\\*\\fldinst{HYPERLINK \"https://www.gnu.org/licenses/\"}}{\\fldrslt{https://www.gnu.org/licenses/\\ul0\\cf0}}}}\\f0\\fs24 >.\\par\
\
\\pard\\widctlpar\\sa160\\sl252\\slmult1\\cf0\\fs22\\par\
Includes icons by icons8.com <{{\\field{\\*\\fldinst{HYPERLINK \"https://www.icons8.com\"}}{\\fldrslt{https://www.icons8.com\\ul0\\cf0}}}}\\f0\\fs22 >.\\par\
\
\\pard\\sa200\\sl276\\slmult1\\f1\\lang10\\par\
}";

	TMemoryStream *oMememoryStream;
	oMememoryStream = new TMemoryStream();
	if (oMememoryStream)
	{
		oMememoryStream->Write(acRtf, sizeof(acRtf) - 1);
		oMememoryStream->Position = 0;
		mmoLicense->Lines->LoadFromStream(oMememoryStream, TEncoding::UTF8);
		delete oMememoryStream;
	}
}


//---------------------------------------------------------------------------
void __fastcall TfrmAbout::EasterTimer(TObject *Sender)
{
	TCanvas *oCanvas;


	if ((!imgEaster) && (!tmrEaster))
	{
		//ToDo: Should not be needed
		butClose->Hide();
		mmoLicense->Hide();

		//ToDo: We should handle window resizing or prevent it while in easter egg
		BorderStyle = bsSingle;
		BorderIcons = BorderIcons << biMaximize;
	
		//Initialize
		imgEaster = new TImage(this);
		tmrEaster = new TTimer(this);
		imgEaster->Parent = this;
		imgEaster->Top = 0;
		imgEaster->Left = 0;
		imgEaster->Width = ClientWidth;
		imgEaster->Height = ClientHeight;

		tmrEaster->Interval = 15;
		tmrEaster->OnTimer = EasterTimer;
		oCanvas = imgEaster->Canvas;

		//Screeen
		oCanvas->Brush->Color = clWhite;
		oCanvas->FillRect(Rect(32, 32, ClientWidth - 32, ClientHeight - 32));
		imgEaster->OnDblClick = EasterClick;

		oCanvas->TextOut(32, 32, "Program: " + Application->Title);
		oCanvas->TextOut(32, 48, "Bytes:   Version " + (String) clsUtil::ExeVersion(Application->ExeName.c_str()));
		TCHAR acName[256];
		clsUtil::GetFileVersionField(Application->ExeName.c_str(), (const TCHAR *) _T("LegalCopyright"), acName, (sizeof(acName) / sizeof(TCHAR)) - 1);
		oCanvas->TextOut(32, 64, "Bytes:   " + (String) acName);
	}
	else
	{
		oCanvas = imgEaster->Canvas;
	}

	//Draw lines
	for (unsigned int iLine = 0; iLine < ClientHeight; iLine++)
	{
		//int iRnd = clsUtil::Random(0, 1);
		unsigned int iRnd = (unsigned int) random(2);


		if (iRnd == 0)
		{
			oCanvas->Pen->Color = clBlue;
		}
		else
		{
			oCanvas->Pen->Color = clYellow;
		}

		if ((iLine < 32) || (iLine > (ClientHeight - 32)))
		{
			oCanvas->MoveTo(0, (int) iLine);
			oCanvas->LineTo(imgEaster->Width, (int) iLine);
		}
		else
		{
			oCanvas->MoveTo(0, (int) iLine);
			oCanvas->LineTo(32, (int) iLine);
			oCanvas->MoveTo(ClientWidth - 32, (int) iLine);
			oCanvas->LineTo(ClientWidth, (int) iLine);
		}
	}
	//Beep(830, 100);
}



//---------------------------------------------------------------------------
void __fastcall TfrmAbout::EasterClick(TObject *Sender)
{
	if (imgEaster)
	{
		delete imgEaster;
		imgEaster = nullptr;
	}
	if (tmrEaster)
	{
		delete tmrEaster;
		tmrEaster = nullptr;
    }

	//ToDo: Should not be needed
	butClose->Show();
	mmoLicense->Show();
	BorderStyle = bsSizeable;
	BorderIcons = BorderIcons >> biMaximize;
}



//---------------------------------------------------------------------------
void __fastcall TfrmAbout::imgAboutDblClick(TObject *Sender)
{
	EasterTimer(nullptr);
}




//---------------------------------------------------------------------------
void __fastcall TfrmAbout::butCloseClick(TObject *Sender)
{
	Hide();
	Close();
}



//---------------------------------------------------------------------------
void __fastcall TfrmAbout::lblCopyrightClick(TObject *Sender)
{
	ShellExecute(Handle, _T("open"), KS_APP_URL, _T(""), _T(""), SW_SHOWNORMAL);
}


//---------------------------------------------------------------------------
void __fastcall TfrmAbout::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
	//VCL does not handle default/cancel in the close button because of the focus on the memo, so let's simulate
	if ((Key == VK_ESCAPE) || (Key == VK_RETURN))
	{
		Close();
	}
}





//---------------------------------------------------------------------------
void __fastcall TfrmAbout::FormClose(TObject *Sender, TCloseAction &Action)
{
	clsUtil::SaveForm(this);
}


//---------------------------------------------------------------------------

